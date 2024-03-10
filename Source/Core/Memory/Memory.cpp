#include "Core/Memory/Memory.h"

#include "Core/CoreConfig.h"
#include "Core/Memory/MemoryManager.h"
#include "Core/Memory/MemoryTypes.h"

#ifdef CUSTOM_MEMORY_ALLOC

CORE_API FMalloc* GAlloc = nullptr;

#endif


void* FUseSystemMallocForNew::operator new (size_t size)
{
	return FMemory::SystemMalloc(size);
}

void FUseSystemMallocForNew::operator delete(void* ptr)
{
	FMemory::SystemFree(ptr);
}

void* FUseSystemMallocForNew::operator new[](size_t size)
{
	return FMemory::SystemMalloc(size);
}

void FUseSystemMallocForNew::operator delete [](void* ptr)
{
	FMemory::SystemFree(ptr);
}

namespace ishak::Memory
{
	FPoolAllocator::FPoolAllocator(
		EngineMemory* engineMemory,
		size_t chunksPerBlock)
			: mChunksPerBlock(chunksPerBlock)
			, mAvailableMemory(engineMemory->PermanentMemory)
			, mAvailableMemorySize(engineMemory->PermanentMemorySize)
	{
	}

	void* FPoolAllocator::Malloc(size_t size)
	{
		if(mAllocationPtr == nullptr)
		{
			// Allocate the Block
			mAllocationPtr = static_cast<Chunk*>(mAvailableMemory);
			// TODO change size for sizeof(Chunk)?
			const size_t tBlockSize = mChunksPerBlock * (sizeof(Chunk) + size) ;

			mAvailableMemory = static_cast<char*>(mAvailableMemory) + tBlockSize;
		}

		Chunk* chunk = mAllocationPtr;
		for(int i = 0; i < mChunksPerBlock; ++i)
		{
			void* chunkRawMemory = reinterpret_cast<char*>(chunk) + sizeof(Chunk) + size;
			chunk->next = static_cast<Chunk*>(chunkRawMemory);
			chunk = chunk->next;
		}

		Chunk* freeChunk = mAllocationPtr;
		auto toReturn = static_cast<void*>(reinterpret_cast<char*>(freeChunk) + sizeof(Chunk));
		mAllocationPtr = mAllocationPtr->next;

		return toReturn;
	}

	void FPoolAllocator::Free(void* ptr)
	{
		Chunk* toFree = reinterpret_cast<Chunk*>(static_cast<char*>(ptr) - sizeof(Chunk));
		toFree->next = mAllocationPtr;

		mAllocationPtr = toFree;
	}

	void* FPoolAllocator::Malloc_Debug(size_t size, ishak::Memory::MemoryBlock*& allocatedBlocks,
		size_t& allocatedBlocksNum)
	{
		return nullptr;
	}

#pragma region HEAP_POOL_ALLOCATOR

	FHeapPoolAllocator::FHeapPoolAllocator(
		EngineMemory* engineMemory,
		size_t chunksPerBlock)
			: mMemory(engineMemory)
			, mChunksPerBlock()
	{
		//Create every block of memory with every chunk

	}

	void* FHeapPoolAllocator::Malloc(size_t size)
	{
		for(size_t i = 0; i < mchunksSize; ++i)
		{
			
		}

		return nullptr;
	}

	void FHeapPoolAllocator::Free(void* ptr)
	{

	}

	void* FHeapPoolAllocator::Malloc_Debug(
		size_t size,
		ishak::Memory::MemoryBlock*& allocatedBlocks,
		size_t& allocatedBlocksNum)
	{
		return nullptr;
	}

#pragma endregion HEAP_POOL_ALLOCATOR


	FEngineMalloc::FEngineMalloc(EngineMemory const& memory)
		: m_engineMemory(memory)
	{
		CreateBlocks();
	}

	void FEngineMalloc::CreateBlocks()
	{
		// We will separate the memory in 2 chunks, one for the Memory Blocks and the other for
		// the memory that we will allocate.

		//   ------------------------------
		//  | Blocks ||   *************    |
		//   ------------------------------

		// PERMANENT MEMORY, for now we do not have in mind padding, that is the next implementation.
		// TODO Padding.

		// 1. Calculate the max blocks that we can allocate, Block struct along raw memory.
		// single block of raw memory that will allocate data. In this case the raw memory will be sized
		// to the memory bus. For this platform and for a generic standpoint, we will set it to 8 bytes(64 bits).

		constexpr size_t memory_block_size = sizeof(MemoryBlock);
		const size_t total_engine_permanent_memory = m_engineMemory.PermanentMemorySize;

		constexpr size_t total_size_to_allocate = memory_block_size + MemoryBlock::SIZE/* memory bus size */;
		const unsigned long long total_blocks = total_engine_permanent_memory / total_size_to_allocate;

		void* memoryIt{ m_engineMemory.PermanentMemory };

		// 2. Allocate the blocks all together
		MemoryBlock* memBlocks = new (memoryIt) MemoryBlock[total_blocks];

		// TODO Maybe struct
		m_memoryBlocks = memBlocks;
		m_blocksNum = total_blocks;

		// 3. Move the memoryIt all the way to the raw memory start.
		// TODO Test with void*
		memoryIt = static_cast<char*>(memoryIt) + (total_blocks * memory_block_size);

		// 4. Associate the Memory to the Blocks.
		for (size_t idx = 0; idx < total_blocks; ++idx)
		{
			MemoryBlock* block = &memBlocks[idx];
			block->initialAddress = memoryIt;

			memoryIt = static_cast<char*>(memoryIt) + MemoryBlock::SIZE;
		}

	}

	void* FEngineMalloc::Malloc(size_t size)
	{
		// We try to find the block that matches the free contiguous memory.
		// TODO Find a better way for this I guess.

		for (size_t idx = 0; idx < m_blocksNum; ++idx)
		{
			MemoryBlock& block = { m_memoryBlocks[idx] };

			// TODO FN = is blocked aligned with size?
			// - if size larger than block, it should be ALL free.
			// - if size smaller than block, it should have that size free
			//			- memory address with module %  == 0;

			size_t a;
			if(void* add = TryAllocateInBlock(&block, size, a))
			{
				return add;
			}
		}

		// TODO Exception BadAlloc
		return nullptr;
	}

#if WITH_TESTS
	void* FEngineMalloc::Malloc_Debug(
		size_t size,
		ishak::Memory::MemoryBlock*& allocatedBlocks,
		size_t& allocatedBlocksNum)
	{
		// We try to find the block that matches the free contiguous memory.
		// TODO Find a better way for this I guess.

		for (size_t idx = 0; idx < m_blocksNum; ++idx)
		{
			MemoryBlock* block = { &m_memoryBlocks[idx] };

			// TODO FN = is blocked aligned with size?
			// - if size larger than block, it should be ALL free.
			// - if size smaller than block, it should have that size free
			//			- memory address with module %  == 0;
			size_t allocatedSize = 0;
			if (void* add = TryAllocateInBlock(block, size, allocatedSize))
			{
				allocatedBlocks = block;
				// For now as we are just allocating stuff less than the cache line, this can be like this.
				allocatedBlocksNum = 1;

				return add;
			}
		}

		// TODO Exception BadAlloc
		return nullptr;
	}
#endif


	void* FEngineMalloc::TryAllocateInBlock(MemoryBlock* block, size_t size, size_t& allocatedSize)
	{
		// CASES:
		// 1.For smaller than cache line. size module fits block.
		// 
		for(size_t idx = 0; idx < block->contiguousAvailableBytes.size(); ++idx)
		{
			if(block->contiguousAvailableBytes[idx] >= size)
			{
				size_t contiguousbytes = 0;
				size_t freeByteIdx= 0;
				bool bSetFreeIdx = false;
				// Free contiguous, so allocate.
				for(size_t byteIdx = 0; byteIdx < block->availableBytes.size(); ++byteIdx)
				{
					if(!block->availableBytes.test(byteIdx))
					{
						if(bSetFreeIdx)
						{
							if((++contiguousbytes) == size)
							{
								break;
							}
							else
							{
								continue;
							}
						}

						if((byteIdx == 0) || (!bSetFreeIdx && ((size % byteIdx) == 0)))
						{
							// set the free address ptr.
							freeByteIdx = idx;
							bSetFreeIdx = true;
						}

						contiguousbytes++;
					}
					else
					{
						// Reset the free address ptr.
						bSetFreeIdx = false;
						contiguousbytes = 0;
					}
				}

				if(bSetFreeIdx)
				{
					SetNewFreeBytesArray(block, size, freeByteIdx);
					void* allocatedMemory =  static_cast<void*>(static_cast<char*>(block->initialAddress) + freeByteIdx);

					m_allocatedMemoryTracker.insert(std::make_pair(allocatedMemory, block));
				}

			}
		}

		return nullptr;
	}

	void FEngineMalloc::Free(void* ptr)
	{
		
	}

	void FEngineMalloc::SetNewFreeBytesArray(MemoryBlock* block, size_t size, size_t freeByteIdx)
	{
		// TODO FN
		// We have to iterate through the block again, so we set the bytes as occupied
		// TODO Rename this
		// Set the bytes that are not available in the bitset.
		// TODO set bytes not available and free bytes array at once.
		size_t byteSizeIdx = 0;
		size_t byteIdx = freeByteIdx;
		while (byteSizeIdx < size)
		{
			block->availableBytes.set(byteIdx, true);

			byteSizeIdx++;
			byteIdx++;
		}

		// TODO Iterate through the memory and when found
		std::array<uint8, MemoryBlock::SIZE> freeBytesArray{};
		size_t freeBytesArrayIdx = 0;
		uint8 contiguousFreeBytesCounter = 0;

		// TODO REFACTOR method here 
		// Create the new free bytes array.
		for (size_t bytesToSetIdx = 0; bytesToSetIdx < block->availableBytes.size(); ++bytesToSetIdx)
		{

			if (!block->availableBytes.test(bytesToSetIdx))
			{
				// Available byte
				contiguousFreeBytesCounter++;
			}
			else
			{
				// Not Available byte
				if (contiguousFreeBytesCounter > 0)
				{
					freeBytesArray[freeBytesArrayIdx++] = contiguousFreeBytesCounter;
					contiguousFreeBytesCounter = 0;
				}
			}
		}

		if (contiguousFreeBytesCounter > 0)
		{
			freeBytesArray[freeBytesArrayIdx] = contiguousFreeBytesCounter;
			contiguousFreeBytesCounter = 0;
		}

		// Set the new free bytes array.
		block->contiguousAvailableBytes = freeBytesArray;
	}


}//ishak::Memory

