#pragma once

// In this file I am going to add the different definitions/classes related to the memory
// We can say that the allocation of memory along other stuff.

#include <array>

#include "Core/CoreMinimal.h"
#include "Core/CoreConfig.h"
#include <bitset>

// Global memory operator override
#define MemoryPageSize 4096
#define MemoryPages(v) (int)v * MemoryPageSize
#define Byte(v) ((uint64)(v))
#define Kilobytes(v) (Byte(v) * 1024)
#define Megabytes(v) (Kilobytes(v) * 1024)
#define Gigabytes(v) (Megabytes(v)) * 1024)

// TODO Check platform if I ever do this for another plat than x64
#define CACHE_LINE_SIZE 8U

	
namespace ishak{ namespace Memory{

struct CORE_API EngineMemory
{
	uint64 PermanentMemorySize;
	void* PermanentMemory;

	uint64 TransientMemorySize;
	void* TransientMemory;
};

}}// ishak::Memory

#define CUSTOM_MEMORY_ALLOC

#ifdef CUSTOM_MEMORY_ALLOC

CORE_API extern class FMalloc* GAlloc;

// For now this is okay, but We will request some memory chunk beforehand for the Engine
// systems memory usage.
class CORE_API FUseSystemMallocForNew
{
public:
	virtual ~FUseSystemMallocForNew() = default;

	void* operator new(size_t size);

	void operator delete(void* ptr);

	void* operator new[](size_t size);

	void operator delete[](void* ptr);
};

namespace ishak::Memory {

	struct CORE_API MemoryBlock
	{
		static constexpr size_t SIZE = CACHE_LINE_SIZE;
		typedef std::array<uint8, SIZE> MemoryBytesContainerT;

		MemoryBlock()
		{
			SetDefaultAvailableBytes();
		}

		MemoryBlock(void* ptr)
			: initialAddress(ptr)
		{
			SetDefaultAvailableBytes();
		}

	private:
		void SetDefaultAvailableBytes()
		{
			contiguousAvailableBytes[0] = SIZE;
		}

	public:

		/* ptr to the init address of the block. */
		void* initialAddress{ nullptr };

		/* Memory that has contiguous space. */
		// TODO Implement TArray with FIXED Size.
		MemoryBytesContainerT contiguousAvailableBytes;

		/* Bitset representing the memoryBlock. 1 occupied. 0 not. */
		std::bitset<SIZE> availableBytes;
	};

}// ishak::Memory

class CORE_API FMalloc : public FUseSystemMallocForNew
{
public:
	virtual void* Malloc(size_t size) = 0;

	virtual void Free(void* ptr) = 0;

#if WITH_TESTS
	virtual void* Malloc_Debug(
		size_t size,
		ishak::Memory::MemoryBlock*& allocatedBlocks,
		size_t& allocatedBlocksNum) = 0;
#endif

};


namespace ishak::Memory
{
	class CORE_API FPoolAllocator : public FMalloc
	{
		struct Chunk
		{
			Chunk* next{ nullptr };
		};

	public:
		FPoolAllocator(
			EngineMemory* engineMemory,
			size_t chunksPerBlock);

		void* Malloc(size_t size) override;
		void Free(void* ptr) override;

		void* Malloc_Debug(
			size_t size,
			ishak::Memory::MemoryBlock*& allocatedBlocks,
			size_t& allocatedBlocksNum) override;

	private:
		size_t mChunksPerBlock{ 0 };
		Chunk* mAllocationPtr{ nullptr };

		void* mAvailableMemory{ nullptr };
		size_t mAvailableMemorySize{ 0 };
	};


	class CORE_API FHeapPoolAllocator : public FMalloc
	{
		struct Chunk
		{
			size_t size;
			bool bUsed;
			Chunk* next;
			word_t data[1];
		};

	public:
		FHeapPoolAllocator(
			EngineMemory* engineMemory,
			size_t chunksPerBlock);

		void* Malloc(size_t size) override;
		void Free(void* ptr) override;

		void* Malloc_Debug(
			size_t size,
			ishak::Memory::MemoryBlock*& allocatedBlocks,
			size_t& allocatedBlocksNum) override;

	private:

		static constexpr size_t mchunksSize = 6;
		Chunk* mChunks[mchunksSize] =
		{
			// 8
			nullptr,
			//16
			nullptr,
			//32,
			nullptr,
			//64
			nullptr,
			//128
			nullptr,
			// > 128
			nullptr
		};

		void* mMemory{ nullptr };
		size_t mChunksPerBlock{ 0 };
		size_t mAvailableMemorySize{ 0 };
	};


	class CORE_API FEngineMalloc : public FMalloc
	{

	public:
		FEngineMalloc() = default;
		FEngineMalloc(EngineMemory const& memory);

		void* Malloc(size_t size) override;

#if WITH_TESTS
		 void* Malloc_Debug(
			 size_t size,
			 ishak::Memory::MemoryBlock*& allocatedBlocks,
			 size_t& allocatedBlocksNum) override;
#endif // WITH_TESTS

		void Free(void* ptr) override;

	private:
		void SetNewFreeBytesArray(MemoryBlock* block, size_t size, size_t freeByteIdx);
		void CreateBlocks();
		void* TryAllocateInBlock(MemoryBlock* block, size_t size, size_t& allocatedSize);

		MemoryBlock* GetMemoryBlocksContainer()
		{
			return m_memoryBlocks;
		}

		size_t GetMemoryBlocksNum() const
		{
			return m_blocksNum;
		}


	private:
		EngineMemory m_engineMemory;
		MemoryBlock* m_memoryBlocks{ nullptr };
		// TODO change for a HashMap
		// Keeps track of allocations, when wanting to deallocate a ptr, we can access the
		// Memory Block and set the pertinent values there.
		std::unordered_map<void*, MemoryBlock*> m_allocatedMemoryTracker;
		size_t m_blocksNum;
	};
}


#endif
