#pragma once 
#include "Core/CoreConfig.h"
#include "Core/TypeTraits/TypeTraits.h"
#include <type_traits>

#if WINDOWS
	#include "Windows.h"
#endif



namespace ishak::memory{

	// General doc for implementing and testing different allocators.

	enum class EMemoryBlockFlags : uint8
	{
		MEM_FromSplited = 1 << 7,
		MEM_InUse = 1 << 6
	};



	// Packed 24 bit Block.
	struct CORE_API Block
	{
		Block* next{ nullptr }; // 8 bytes
		uint16 size{ 0 }; // 2 bytes
		uint8 flags{ 0 }; // 1 byte
		uint8 _pad[5]; // 5 bytes
		word_t data[1]; // 8 bytes
	};


	inline void SetFlag(Block* b, EMemoryBlockFlags flag, const bool bValue)
	{
		uint8 flagValueToAdd = 0;
		if (bValue)
		{
			flagValueToAdd = (uint8)(flag);
			b->flags |= flagValueToAdd;
		}
		else
		{
			flagValueToAdd = ~(uint8)(flag);
			b->flags &= flagValueToAdd;
		}
	}



	inline bool IsBlockFromSplitted(Block* b)
	{
		return b->flags & (uint8)(EMemoryBlockFlags::MEM_FromSplited);
	}

	inline bool IsBlockInUse(Block* b)
	{
		return b->flags & ((uint8)(EMemoryBlockFlags::MEM_InUse));
	}

	inline size_t align(size_t n)
	{
		return (n + sizeof(word_t) - 1) & ~(sizeof(word_t) - 1);
	}



	namespace helper
	{
		Block* getHeader(word_t* data);
	}

	class CORE_API Allocator
	{
	public:
		virtual ~Allocator() = default;

		virtual void* Allocate(size_t size) = 0;
		virtual void Free(void* ptr) = 0;

		inline size_t allocSize(size_t size)
		{
			return size + sizeof(Block) - sizeof(std::declval<Block>().data);
		}
	};


	// NOTE**: Having the Block overhead when calculating Free memory.
	/*
	 * Allocator that finds the first available Block that corresponds to the
	 * requested size for the allocation.
	 *
	 * ALLOCATION: Equal or greater in size.
	 */
	class CORE_API FirstFitAllocator : public Allocator
	{
		Block* mHeapStart = nullptr;
		Block* mTop = nullptr;

	public:
		FirstFitAllocator() = default;

		~FirstFitAllocator();

		void* Allocate(size_t size) override;
		void Free(void* ptr) override;

	private:
		Block* CreateBlock(size_t size);
		Block* RequestFromOS(size_t Size);
		Block* FindBlock(size_t size);
	};


	/**
	 *  This allocator works as the FirstFitAllocator, but in this case it caches
	 *  the last allocated block, when iterating again, if the requested size is equal
	 *	or greater to that last allocated block, then, we start searching for the best
	 *	fit for the allocation from the last allocated block, is a way of skipping
	 *	some iterations.
	 *
	 * ALLOCATION: equal or greater in size.
	 *
	 */
	class CORE_API NextFitAllocator : public Allocator
	{
	public:
		NextFitAllocator() = default;
		~NextFitAllocator();

		void* Allocate(size_t size) override;
		void Free(void* ptr) override;
		Block* m_lastAllocatedBlock = nullptr;

	private:
		Block* FindBlock(size_t size);
		Block* CreateBlock(size_t size);

		Block* m_heapStart{ nullptr };
		Block* m_top{ nullptr };
	};


	/**
	 * Allocator that finds the best fit for the requested size, bigger sized blocks will be
	 * divided into smaller blocks if the size requested is smaller than the found block.
	 */
	class CORE_API BestFitAllocator : public Allocator
	{
	public:

		BestFitAllocator() = default;
		~BestFitAllocator();

		/**
		 * When freeing the memory from a block, the next ones, if splitted,
		 * are going to be freed as well.
		 */
		Block* GetNextNotSplitedBlock(Block* b);


		void* Allocate(size_t size) override;
		void Free(void* ptr) override;

		Block* CreateBlock(size_t size);
		Block* FindBlock(size_t size);
		Block* SplitBlock(Block* toSplit, size_t sz);
		void ReconcileBlocks(Block* b);


	private:
		Block* m_top{ nullptr };
		Block* m_heapStart{ nullptr };
		Block* m_lastAllocatedBlock{ nullptr };
	};


	struct FMemoryChunk	
	{
		FMemoryChunk() = default;

		void* m_buffer{ nullptr };
		size_t m_bufferSize{ 0 };
	};


	/** Gets a memory chunk an then allocates the memory needed for it */
	class CORE_API StackAllocator
	{
	public: 
		StackAllocator() = default;
		StackAllocator(FMemoryChunk const& memoryChunk)
			: m_memoryChunk(memoryChunk)
		{

		}

		void* Allocate(size_t size)
		{

		}

		void Free(void* ptr)
		{
			// Stack Allocator does nothing
		}

	private:
		FMemoryChunk m_memoryChunk;
	};
	
	template<typename ElementT, size_t elementsNum, size_t kAlingment = 8>
	class CORE_API FixedSizeAllocator
	{
	public:
		FixedSizeAllocator() = default;
		FixedSizeAllocator(FMemoryChunk const& chunk )
			: m_memoryChunk{ chunk }
		{
			const size_t sizeOfBlock{ sizeof(Block) - sizeof(std::declval<Block>().data) };
			const size_t alignedSize{ (sizeof(ElementT) + kAlingment - 1) & ~(kAlingment - 1)};
			const size_t memoryNeeded{ (alignedSize + sizeOfBlock) * elementsNum };
			if(memoryNeeded > chunk.m_bufferSize)
			{
				// TODO EXCEPTION
				return;
			}

			// CreateBlocks
			size_t blockAndElementSize = sizeOfBlock + alignedSize;
			size_t cretedBlocks = 0;
			Block* blockIdx = nullptr;
			for(char* memoryAddressIdx = (char*)chunk.m_buffer;	
				cretedBlocks < elementsNum;
				memoryAddressIdx + blockAndElementSize)
			{
				Block* block = (Block*)memoryAddressIdx;
				block->size = alignedSize;
				if(m_FirstFreeBlock == nullptr)
				{
					m_FirstFreeBlock = block;
				}

				if(blockIdx != nullptr)
				{
					blockIdx->next = block;
					blockIdx = block;
				}
				else
				{
					blockIdx = block;
				}

				cretedBlocks++;
			}
		}
		
		void* Allocate(size_t size = sizeof(ElementT))
		{
			if(!m_FirstFreeBlock)
			{
				// bad alloc, no more memory
				return nullptr;
			}

			Block* t_freeBlock{ m_FirstFreeBlock };
			m_FirstFreeBlock = t_freeBlock->next;

			return t_freeBlock->data;
		}

		void Free(void* ptr)
		{
			if(ptr == nullptr)
			{
				return;
			}

			Block* block{ helper::getHeader((word_t*)ptr) };
			block->next = m_FirstFreeBlock;
			m_FirstFreeBlock = block;
		}

	private:
		FMemoryChunk m_memoryChunk;
		Block* m_FirstFreeBlock{ nullptr };
	};

	/** 
	* Single Allocated Block of memory. Can run out of memory.
	* 
	* **NOTE**:
	*	Maybe in the future would be nice to explore a solution where we allocate memory in memory addresses with the module of the given address:
	* 
	* 2 -- addresses with module 2
	* 3 -- adddresses with modue 3 
	* so on...
	*/
	template<typename ElementT, size_t kElementsNum, size_t kAlingment = 8>
	class CORE_API InlineAllocator
	{
	public:
		InlineAllocator() = default;
		InlineAllocator(FMemoryChunk const& memoryChunk)
			: m_memoryChunk{ memoryChunk }
		{
			
			const size_t t_objAlignedSize = (sizeof(ElementT) + kAlingment - 1) & ~(kAlingment - 1);
			const size_t t_allocatorBlockSize = t_objAlignedSize * kElementsNum;

			const size_t t_blockSize = sizeof(Block) - sizeof(std::declval<Block>().data);
			const size_t t_toAllocateSize = t_blockSize + t_allocatorBlockSize;

			if(t_toAllocateSize > m_memoryChunk.m_bufferSize)
			{
				// The allocator has not enough memory from the FMemoryChunk to allocate memory.
				throw std::bad_alloc{ };
			}

			m_Block = (Block*)m_memoryChunk.m_buffer;
			m_Block->size = t_allocatorBlockSize;
		}

		~InlineAllocator() = default;

		uint32 GetCapacity()
		{
			return kElementsNum;
		}

		static constexpr size_t GetNeededMemory()
		{
			constexpr size_t t_objAlignedSize = (sizeof(ElementT) + kAlingment - 1) & ~(kAlingment - 1);
			constexpr size_t t_allocatorBlockSize = t_objAlignedSize * kElementsNum;

			constexpr size_t t_blockSize = sizeof(Block) - sizeof(std::declval<Block>().data);
			constexpr size_t t_toAllocateSize = t_blockSize + t_allocatorBlockSize;

			return t_toAllocateSize;
		}

		void* Allocate(size_t size)
		{
			if(m_memoryRequested)
			{
				throw std::bad_alloc{ };
			}
			
			m_memoryRequested = true;
			return m_Block->data;
		}

		void Free(void* ptr)
		{
			// the memory manager is the one that frees this.
		}

		FMemoryChunk m_memoryChunk;
		Block* m_Block{ nullptr };
		bool m_memoryRequested{ false };
	};

	/** Linked list of free blocks of memory, in this case when a block is occupied, we delete it from the 
	* list, so when it is freed again, is put at the top of the list, this is less cache friendlier than having 
	* all the blocks linked together, but we exchange cache friendliness for iteration complexity, because 
	* throguth this way, we only iterate through the free chunks.
	* 
	*/
	class CORE_API BestFitFreeAllocator 
	{
		// Packed 32 bit Block.
		struct Block_Free
		{
			Block_Free* next{ nullptr }; // 8 bytes
			uint16 size{ 0 }; // 2 bytes
			uint8 flags{ 0 }; // 1 byte
			uint8 _pad[5]; // 5 bytes
			word_t data[1]; // 8 bytes
		};


	public:

		BestFitFreeAllocator() = default;
		BestFitFreeAllocator(const FMemoryChunk& chunk);


		~BestFitFreeAllocator();

		uint32 GetCapacity()
		{
			return 0;
		}


		void SetFlag(Block_Free* b, EMemoryBlockFlags flag, const bool bValue)
		{
			uint8 flagValueToAdd = 0;
			if (bValue)
			{
				flagValueToAdd = (uint8)(flag);
				b->flags |= flagValueToAdd;
			}
			else
			{
				flagValueToAdd = ~(uint8)(flag);
				b->flags &= flagValueToAdd;
			}
		}

		bool IsBlockInUse(Block_Free* b)
		{
			return b->flags & ((uint8)(EMemoryBlockFlags::MEM_InUse));
		}

		Block_Free* getHeader(word_t* data)
		{
			return (Block_Free*)((char*)data + sizeof(std::declval<Block_Free>().data) - sizeof(Block_Free));
		}

		bool IsBlockFromSplitted(Block_Free* b)
		{
			return b->flags & (uint8)(EMemoryBlockFlags::MEM_FromSplited);
		}

		void* Allocate(size_t size, size_t alingment = 8);
		void Free(void* ptr);

		size_t GetLeaks() const
		{
			return m_allocations - m_freedAllocations;
		}

		Block_Free* CreateBlock(size_t size);
		Block_Free* FindBlock(size_t size);
		Block_Free* SplitBlock(Block_Free* toSplit, size_t sz);


	private:

		FMemoryChunk m_memoryChunk;
		size_t m_allocatedMemory{ 0 };
		size_t m_allocations{0};
		size_t m_freedAllocations{0};
		Block_Free* m_lastFreeBlock{ nullptr };
	};



	enum class EMemoryHirarchy : uint8
	{
		PERSISTENT,
		TRANSIENT,
		NUM
	};

	enum class EMemorySandBoxType : uint8
	{
		CORE,
		GAMEPLAY,
		NUM
	};


	class CORE_API DefaultAllocator
	{
	public:
		DefaultAllocator() = default;

		void* Allocate(size_t size);
		void Free(void* ptr);
		uint32 GetCapacity()
		{
			return 0;
		}
	};

	template<class T>
	T* NewObject(size_t size, Allocator* allocator)
	{
		T* obj = (T*)(allocator->Allocate(size));
		new (obj) T;

		return obj;
	}

	/**
	* We are going to request some pages from the OS and then divide it into hirarchies.
	*
	* The memory will be divided into different hirachies, and sandboxes, each sandbox can be managed
	* by different Allocators.
	*/
	class CORE_API MemoryManager
	{
		static constexpr uint8 kDefaultPagesToRequest = 1U;

	public:
		MemoryManager(size_t memoryToAllocate = 0);
		~MemoryManager();

		struct SandBox
		{
			SandBox() = default;

			void* Allocate(size_t size)
			{
				return allocator.Allocate(size);
			}

			void Free(void* ptr)
			{
				allocator.Free(ptr);
			}

			char* memory{ nullptr };
			size_t size{ 0 };
			BestFitFreeAllocator allocator;
			EMemorySandBoxType type;
			uint8 _pad[7];
		};


		template<class T>
		T* NewObject(size_t size, EMemoryHirarchy hirarchy, EMemorySandBoxType sandBoxType)
		{
			T* obj = (T*)(GetMemorySandBox(hirarchy, sandBoxType));
			new (obj) T;

			return obj;
		}

		SandBox* GetMemorySandBox(EMemoryHirarchy hirarchy, EMemorySandBoxType type)
		{
			return &sandboxes[(uint8)hirarchy][(uint8)type];
		}

		void* hirarchies[(uint8)EMemoryHirarchy::NUM];
		SandBox sandboxes[(uint8)EMemoryHirarchy::NUM][(uint8)EMemorySandBoxType::NUM];
	};

	struct System
	{
		System() = default;



	};

}// ishak::memory
