#include "Core/Memory/Allocator.h"

#include <assert.h>
#include <Core/Memory/Memory.h>

namespace ishak::memory{

	Block* helper::getHeader(word_t* data)
	{
		return (Block*)((char*)data + sizeof(std::declval<Block>().data) - sizeof(Block));
	}

	FirstFitAllocator::~FirstFitAllocator()
	{
		Block* blockIt = mHeapStart;

		while(blockIt != nullptr)
		{
			Block* cachedNext = blockIt->next;
			assert(VirtualFree(blockIt, 0, MEM_RELEASE) != 0);
			blockIt = cachedNext;
		}
	}

	void* FirstFitAllocator::Allocate(size_t size)
	{
		auto block{ FindBlock(size) };
		if(block)
		{
			return block->data;
		}

		block = CreateBlock(size);


		if (!mHeapStart)
		{
			mHeapStart = block;
		}

		// Link Blocks
		// mTop is last allocated block
		if(mTop != nullptr)
		{
			mTop->next = block;
		}

		mTop = block;

		return block->data;
	}


	void FirstFitAllocator::Free(void* ptr)
	{
		Block* block{ helper::getHeader((word_t*)ptr) };
		SetFlag(block, EMemoryBlockFlags::MEM_InUse, false);
	}

	Block* FirstFitAllocator::CreateBlock(size_t size)
	{
		// If we are not intending to use a memory arena
		// we can use this method because the memory
		// is always being returned from different points in the heap
		// , BUT when using a memory arena, we have to request the
		// block size + the obj size.
		const size_t alignedSize{ align(size) };
		Block* createdBlock = RequestFromOS(alignedSize);

		createdBlock->size = alignedSize;
		SetFlag(createdBlock, EMemoryBlockFlags::MEM_InUse, true);

		return createdBlock;
	}

	Block* FirstFitAllocator::RequestFromOS(size_t Size)
	{
		auto block = (Block*)VirtualAlloc(
			0,
			Size,
			MEM_RESERVE | MEM_COMMIT,
			PAGE_READWRITE
		);

		// check out OOM
		// In this case this is not gonna happen, when having large arena,we could.

		return block;
	}

	Block* FirstFitAllocator::FindBlock(size_t size)
	{
		Block* foundBlock{ mHeapStart };

		while(foundBlock != nullptr)
		{
			// for now even the block size is greated, we allocate memory
			if(IsBlockInUse(foundBlock)|| foundBlock->size < size)
			{
				foundBlock = foundBlock->next;
				continue;
			}

			return foundBlock;
		}

		// NO BLOCKS AVAILABLE
		return nullptr;
	}

	NextFitAllocator::~NextFitAllocator()
	{
		Block* blockIt = m_heapStart;

		while(blockIt != nullptr)
		{
			Block* cachedNext = blockIt->next;
			assert(VirtualFree(blockIt, 0, MEM_RELEASE) != 0);
			blockIt = cachedNext;
		}

	}

	void* NextFitAllocator::Allocate(size_t size)
	{
		// Find Block
		Block* block = FindBlock(size);
		if(!block)
		{
			block = CreateBlock(size);
		}

		SetFlag(block, EMemoryBlockFlags::MEM_InUse, true);

		if(m_heapStart == nullptr)
		{
			m_heapStart = block;
		}

		m_lastAllocatedBlock = block;

		return block->data;
	}

	void NextFitAllocator::Free(void* ptr)
	{
		Block* block{ helper::getHeader((word_t*)ptr) };
		SetFlag(block, EMemoryBlockFlags::MEM_InUse, false);
	}

	Block* NextFitAllocator::FindBlock(size_t size)
	{
		// Checking which is the best first idx for the iteration.
		Block* blockIt = nullptr;
		bool bNeedToReiterate = false;
		if(m_lastAllocatedBlock == nullptr || m_lastAllocatedBlock->size < size)
		{
			blockIt = m_heapStart;
		}
		else
		{
			blockIt = m_lastAllocatedBlock;
			bNeedToReiterate = true;
		}

		while(blockIt != nullptr)
		{
			if(IsBlockInUse(blockIt) || blockIt->size < size) 
			{
				blockIt = blockIt->next;
				continue;
			}

			return blockIt;
		}

		// If not found and the finding is not from the start of the list, try to find it.
		if(bNeedToReiterate)
		{
			blockIt = m_heapStart;
			while(blockIt != m_lastAllocatedBlock)
			{
				if(IsBlockInUse(blockIt) || blockIt->size < size )
				{
					blockIt = blockIt->next;
					continue;
				}

				return blockIt;
			}
		}

		return nullptr;
	}

	Block* NextFitAllocator::CreateBlock(size_t size)
	{
		const size_t alignedSize = align(size);
		Block* block = (Block*)VirtualAlloc(
			nullptr,
			alignedSize,
			MEM_COMMIT | MEM_RESERVE,
			PAGE_READWRITE);

		block->size = alignedSize;

		if (m_top)
		{
			m_top->next = block;
		}

		m_top = block;

		return block;
	}

	BestFitAllocator::~BestFitAllocator()
	{
		Block* it{ m_heapStart };


		// In the pool we are going to Free all the memory at once,
		// so this will not be needed.
		while(it != nullptr)
		{
			Block* cachedNext{ GetNextNotSplitedBlock(it) };
			
			assert(VirtualFree(it, 0, MEM_RELEASE) != 0);
			it = cachedNext;
			// By the documentation, the dwSize must be 0.
		}
	}

	Block* BestFitAllocator::GetNextNotSplitedBlock(Block* b)
	{
		if(b)
		{
			Block* nonSplitedBlock{ b->next };
			while(nonSplitedBlock && IsBlockFromSplitted(nonSplitedBlock))
			{
				nonSplitedBlock = nonSplitedBlock->next;
			}

			return nonSplitedBlock;
		}

		return nullptr;
	}

	// BEST_FIT 
	void* BestFitAllocator::Allocate(size_t size)
	{
		Block* block{ FindBlock(size) };
		if(!block)
		{
			block = CreateBlock(size);
		}

		SetFlag(block, EMemoryBlockFlags::MEM_InUse, true);
		m_lastAllocatedBlock = block;
		
		return block->data;
	}

	Block* BestFitAllocator::FindBlock(size_t size)
	{
		Block* blockIt{ nullptr };
		bool bNeedToReiterate = false;

		if(m_lastAllocatedBlock && m_lastAllocatedBlock->size >= size)
		{
			blockIt = m_lastAllocatedBlock;
			bNeedToReiterate = true;
		}
		else
		{
			blockIt = m_heapStart;
		}

		// Iterate to the end
		while(blockIt != nullptr)
		{
			// Best fit search with splitting a larger block.

			if(!IsBlockInUse(blockIt) && blockIt->size > size)
			{
				// Split
				if(Block* splittedBlock = SplitBlock(blockIt, size))
				{
					return splittedBlock;
				}

			}

			if(IsBlockInUse(blockIt) || blockIt->size != size)
			{
				blockIt = blockIt->next;
				continue;
			}

			return blockIt;
		}

		if(bNeedToReiterate)
		{
			blockIt = m_heapStart;
			while(blockIt != m_lastAllocatedBlock)
			{
				if (!IsBlockInUse(blockIt) && blockIt->size > size)
				{
					// Split
					if (Block* splittedBlock = SplitBlock(blockIt, size))
					{
						return splittedBlock;
					}

				}

				if(IsBlockInUse(blockIt) || blockIt->size != size)
				{
					blockIt = blockIt->next;
					continue;
				}

				return blockIt;
			}
		}

		return nullptr;
	}

	Block* BestFitAllocator::SplitBlock(Block* toSplit, size_t sz)
	{

		// We make sure that in the current space there is space for at lease a block.
		if(auto desiredSize = toSplit->size - sz; desiredSize >= sizeof(Block))
		{
			// Current block size is replaced with the new one and we create another one with the remaining space.
			const auto splittedBlockSize = desiredSize - (sizeof(Block) - sizeof(std::declval<Block>().data));
			toSplit->size = sz;
			Block* cachedPrevNext = toSplit->next;

			Block* newBlock = (Block*)(((char*)(toSplit->data)) + toSplit->size);
			newBlock->size = splittedBlockSize;
			newBlock->next = cachedPrevNext;
			SetFlag(newBlock, EMemoryBlockFlags::MEM_FromSplited, true);

			toSplit->next = newBlock;

			return toSplit;
		}

		return nullptr;
	}


	Block* BestFitAllocator::CreateBlock(size_t size)
	{
		const size_t alignedSize{ align(size) };

		Block* block{ (Block*)VirtualAlloc(
			NULL,
			alignedSize,
			MEM_COMMIT | MEM_RESERVE,
			PAGE_READWRITE) };

		assert(block != nullptr);

		block->size = size;

		if(m_heapStart == nullptr)
		{
			m_heapStart = block;
		}

		// Link to next
		if(m_top)
		{
			m_top->next = block;
		}

		m_top = block;
		return block;
	}

	void BestFitAllocator::Free(void* ptr)
	{
		Block* block = helper::getHeader((word_t*)ptr);
		SetFlag(block, EMemoryBlockFlags::MEM_InUse, false);
		ReconcileBlocks(block);
	}

	void BestFitAllocator::ReconcileBlocks(Block* b)	
	{
		if(b->next && !IsBlockInUse(b->next))
		{
			// Overhead for making this and then when requested lower sized block, 
			// we have to split it it again? Maybe limiting the amount of blocks that we can have
			// and then if not found, we iterate through all and merge when possible.

			b->size += b->next->size + (sizeof(Block) - sizeof(std::declval<Block>().data));
			b->next = b->next->next;
		}
	}


	BestFitFreeAllocator::BestFitFreeAllocator(const FMemoryChunk& chunk)
		: m_memoryChunk(chunk)
	{

	}

	BestFitFreeAllocator::~BestFitFreeAllocator()
	{
		// TODO Just for testing 
		if(m_memoryChunk.m_buffer)
		{
			return;
		}

		Block_Free* it{ m_lastFreeBlock };

		// In the pool we are going to Free all the memory at once,
		// so this will not be needed.
		while (it != nullptr)
		{
			Block_Free* cachedPrev = it->next;
			assert(VirtualFree(it, 0, MEM_RELEASE) != 0);
			it = cachedPrev;
			// By the documentation, the dwSize must be 0.
		}
	}

	// BEST_FIT 
	void* BestFitFreeAllocator::Allocate(size_t size, size_t alingment)
	{
		const size_t alignedSize = (alingment + size - 1) & ~(alingment - 1);
		Block_Free* block{ FindBlock(alignedSize) };
		if (!block)
		{
			block = CreateBlock(alignedSize);
		}

		SetFlag(block, EMemoryBlockFlags::MEM_InUse, true);
		m_allocations++;

		return block->data;
	}

	BestFitFreeAllocator::Block_Free* BestFitFreeAllocator::FindBlock(size_t size)
	{

		if(m_lastFreeBlock)
		{
			if (m_lastFreeBlock->size == size)
			{
				m_lastFreeBlock = m_lastFreeBlock->next;
				return m_lastFreeBlock;

			}
			else if (m_lastFreeBlock->size > size)
			{
				if(auto splittedBlock =  SplitBlock(m_lastFreeBlock, size))
				{					
					m_lastFreeBlock = splittedBlock->next;
					return splittedBlock;
				}
			}


			auto blockIt{ m_lastFreeBlock };

			// Iterate to the end
			while (blockIt->next != nullptr)
			{
				// Best fit search with splitting a larger block.

				if(blockIt->next->size > size)
				{
					if(Block_Free* block = SplitBlock(blockIt->next, size))
					{
						blockIt->next = block->next;

						return block;
					}
				}

				if (blockIt->next->size == size)
				{
					return blockIt->next;
				}

				blockIt = blockIt->next;
			}
		}

		return nullptr;
	}

	BestFitFreeAllocator::Block_Free* BestFitFreeAllocator::SplitBlock(Block_Free* toSplit, size_t sz)
	{
		// We make sure that in the current space there is space for at lease a block.
		if (auto desiredSize = toSplit->size - sz; desiredSize >= sizeof(Block_Free))
		{
			// Current block size is replaced with the new one and we create another one with the remaining space.
			const auto splittedBlockSize = desiredSize - (sizeof(Block_Free) - sizeof(std::declval<Block_Free>().data));
			toSplit->size = sz;

			Block_Free* newBlock = (Block_Free*)(((char*)(toSplit->data)) + toSplit->size);
			newBlock->size = splittedBlockSize;
			newBlock->next = toSplit->next;
			SetFlag(newBlock, EMemoryBlockFlags::MEM_FromSplited, true);			

			return toSplit;
		}

		return nullptr;
	}


	BestFitFreeAllocator::Block_Free* BestFitFreeAllocator::CreateBlock(size_t size)
	{	
		void* memoryToUse{ nullptr };

		if(m_memoryChunk.m_buffer)
		{
			if(auto tempMemoryAddress = m_allocatedMemory + size; tempMemoryAddress < m_memoryChunk.m_bufferSize)
			{
				char* buffer = static_cast<char*>(m_memoryChunk.m_buffer);
				buffer += tempMemoryAddress;
				m_allocatedMemory += size;

				memoryToUse = buffer;
			}
			else
			{
				throw std::bad_alloc{ };
			}


		}
		else
		{
			memoryToUse = VirtualAlloc(
				NULL,
				size,
				MEM_COMMIT | MEM_RESERVE,
				PAGE_READWRITE);
		}

		Block_Free* block{ (Block_Free*)(memoryToUse)};
		new (block) Block_Free;
		assert(block != nullptr);

		block->size = size;

		return block;
	}

	void BestFitFreeAllocator::Free(void* ptr)
	{
		Block_Free* block = getHeader((word_t*)ptr);
		SetFlag(block, EMemoryBlockFlags::MEM_InUse, false);
		m_freedAllocations++;

		if (!m_lastFreeBlock) 
		{
			m_lastFreeBlock = block;
			return;
		}

		m_allocatedMemory -= block->size;
		// Link the free block with the last Free Block and viceversa
		block->next = m_lastFreeBlock;
		m_lastFreeBlock = block;
	}
	// ~ BEST FIT ~

	MemoryManager::MemoryManager(size_t memoryToAllocate)
	{
		if(memoryToAllocate == 0)
		{
			memoryToAllocate = MemoryPages(kDefaultPagesToRequest);
		}

		assert(memoryToAllocate > 1);
		const int totalAllocatedPages = memoryToAllocate;
		const int memoryPerHirarchy = totalAllocatedPages / (uint8)(EMemoryHirarchy::NUM);
		const int memoryPerSandBox = memoryPerHirarchy / (uint8)(EMemorySandBoxType::NUM);

		for(uint8 i = 0; i < (uint8)EMemoryHirarchy::NUM; ++i)		
		{
			void* requestedMemory = VirtualAlloc(
				NULL,
				memoryPerHirarchy,
				MEM_RESERVE | MEM_COMMIT,
				PAGE_READWRITE);

			hirarchies[i] = requestedMemory;

			// TODO Divide the sandBoxes
			size_t associatedMemory = 0;
			char* currentBufferPtr = (char*)(requestedMemory);
			for(uint8 j = 0; j < (uint8)(EMemorySandBoxType::NUM); ++j)
			{
				size_t sandBoxSize = memoryPerSandBox;

				// if we are creating the last sandBox
				if(j == (uint8)(EMemorySandBoxType::NUM) - 1)
				{
					int temp = associatedMemory + memoryPerSandBox;
					if (int memoryModuleToEnd = totalAllocatedPages - temp; memoryModuleToEnd < memoryPerSandBox)
					{
						// If there is some space left when we create this SandBox and that space is less than an entire
						// Sandbox, then we add it to the sandBox
						sandBoxSize += memoryModuleToEnd;
					}
				}

				sandboxes[i][j] = SandBox{ currentBufferPtr + associatedMemory, sandBoxSize};

				SandBox& sandbox{ sandboxes[i][j] };
				sandbox.type = (EMemorySandBoxType)(j);
			    sandbox.allocator = BestFitFreeAllocator{ FMemoryChunk{ sandbox.memory, sandbox.size } };				

				associatedMemory += memoryPerSandBox;
			}
		}


		/* We have the SandBoxes, so now the SandBoxes are going to have space Available for the different systems,
		* so what I spect the final prupose of the SandBox is, is to deliver some memory Space that the systems can Take
		* for creating Allocators, and this allocators are going to be passed to the Systems Arrays or ptrs creation.
		*
		* If we want to create a Turret System, we will have to reslove at compile time, the size of the requested space to the 
		* SandBox Allocator.
		*/

		// Every Sandbox will have a BestFitFreeAllocator for handling memory, once a system requests a memory block, this system
		// can create an allocator with this memory.	

		

	}

	MemoryManager::~MemoryManager()
	{
		
		// Free the Hirarchies
		for (uint8 i = 0; i < (uint8)EMemoryHirarchy::NUM; ++i)
		{
			void* hirarchy{ hirarchies[i] };
			assert(VirtualFree(hirarchy, 0, MEM_RELEASE) > 0);
		}
	}

	void* DefaultAllocator::Allocate(size_t size)
	{		
		return std::malloc(size);
	}

	void DefaultAllocator::Free(void* ptr)
	{
		std::free(ptr);
	}

}// ishak::Memory