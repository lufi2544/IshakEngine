
#include "Core/CoreMinimal.h"
#include "doctest.h"

namespace ishak::practice
{

	struct Block
	{
		size_t size;
		bool bFromSplited = false;
		uint8 _pad[7];
		Block* next;
		word_t data[1];
	};


	class CORE_API InlineAllocator
	{
	public:
		InlineAllocator(size_t size)
		{
			const auto toAllocate{ sizeof(Block) - sizeof(std::declval<Block>().data) + size };
			m_block = (Block*)VirtualAlloc(NULL, toAllocate, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
			m_block->size = size;
		}

		~InlineAllocator()
		{
			if(m_block)
			{
				assert(VirtualFree(m_block, 0, MEM_RELEASE) > 0);
			}
		}

		void* Allocate(size_t size, size_t alingment = 8)
		{
			const auto alignedSize{ (size + alingment - 1) & ~(alingment - 1) };
			if(m_offset + alignedSize > m_block->size)
			{
				throw std::bad_alloc{ };
			}

			char* toReturn = (char*)m_block->data + m_offset;

			m_offset += alignedSize;

			return toReturn;
		}

		void Free()
		{

		}

		int m_offset = 0;
		Block* m_block = nullptr;
	};


	TEST_CASE("Inline Allocator,OK")
	{
		InlineAllocator alloc{ Byte(100)};

		int* a = (int*)alloc.Allocate(sizeof(int), alignof(int));
		*a = 22;
		int* b = (int*)alloc.Allocate(sizeof(int), alignof(int));
		*b = 32;


		CHECK(*a == 22);
		CHECK(*b == 32);
		CHECK(alloc.m_offset == (sizeof(int) * 2));
	}



	TEST_CASE("Inline Allocator,OK")
	{
		InlineAllocator alloc{ Byte(10) };

		int* a = (int*)alloc.Allocate(sizeof(int), alignof(int));
		*a = 22;
		int* b = (int*)alloc.Allocate(sizeof(int), alignof(int));
		*b = 32;

		CHECK_THROWS(alloc.Allocate(4, 4), std::bad_alloc{});
	}


	Block* getHeader(void* ptr)
	{
		return reinterpret_cast<Block*>((char*)ptr - (sizeof(Block) - sizeof(std::declval<Block>().data)));
	}

	class CORE_API BestFitAllocator
	{
	public:
		BestFitAllocator() = default;
		~BestFitAllocator() 
		{
			auto it = m_tail;

			while(it != nullptr)
			{
				auto cachedNext = it->next;
				while(cachedNext && cachedNext->bFromSplited)
				{
					cachedNext = cachedNext->next;
				}

				assert(VirtualFree(it, 0, MEM_RELEASE) > 0);
				it = cachedNext;
			}
		}


		void* Allocate(size_t size, size_t alingment = 8) 
		{
			const auto alignedS{ (size + alingment - 1) & ~(alingment - 1) };

			auto it = m_tail;
			if(it)
			{
				while(it != nullptr)
				{
					if (it->size == alignedS)
					{
						return it->data;
					}
					else if (it->size > alignedS)
					{
						const auto splittedSize = it->size - alignedS;
						if (splittedSize >= sizeof(Block))
						{
							Block* splittedBlock{ reinterpret_cast<Block*>((char*)it->data + alignedS) };
							splittedBlock->size = splittedSize - ((sizeof(Block) - sizeof(std::declval<Block>().data)));
							splittedBlock->bFromSplited = true;
							splittedBlock->next = it->next;

							it->size = alignedS;
							it->next = splittedBlock;

							return it->data;
						}
					}

					it = it->next;
				}
			}

			Block* newBlock{ (Block*)VirtualAlloc(NULL, sizeof(Block) - sizeof(std::declval<Block>().data) + alignedS, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE) };
			newBlock->size = alignedS;
			return newBlock->data;
		}

		void Free(void* ptr)
		{
			Block* block{ getHeader(ptr) };
			if(!m_tail)
			{
				m_tail = block;
			}
			else 
			{
				block->next = m_tail;
				m_tail = block;
			}
		}

		Block* m_tail{ nullptr };

	};


	TEST_CASE("BestFitAlloc_0, OK")
	{
		BestFitAllocator alloc;
		auto a = alloc.Allocate(8);
		auto b = alloc.Allocate(8);
		auto c = alloc.Allocate(16);

		alloc.Free(a);
		alloc.Free(b);
		alloc.Free(c);

		auto d = alloc.Allocate(8);
		auto f = getHeader(d);
		auto ff = getHeader(b);

		CHECK(getHeader(d) == getHeader(b));
	}


	TEST_CASE("BestFitAlloc_1, OK")
	{
		BestFitAllocator alloc;
		auto a = alloc.Allocate(4);
		auto b = alloc.Allocate(8);
		auto c = alloc.Allocate(16);

		alloc.Free(a);
		alloc.Free(b);
		alloc.Free(c);

		auto d = alloc.Allocate(4);

		CHECK(getHeader(d) == getHeader(b));
	}

	TEST_CASE("BestFitAlloc_Splitted, OK")
	{
		BestFitAllocator alloc;
		auto a = alloc.Allocate(4);
		auto b = alloc.Allocate(8);
		auto c = alloc.Allocate(MemoryPages(1));

		alloc.Free(a);
		alloc.Free(b);
		alloc.Free(c);

		auto d = alloc.Allocate(4);
		auto f = alloc.Allocate(32);
		auto f_1 = alloc.Allocate(32);
		auto f_2 = alloc.Allocate(32);
		auto f_3 = alloc.Allocate(32);
		auto f_4 = alloc.Allocate(32);

		CHECK(getHeader(d) == getHeader(c));
		CHECK(getHeader(f)->bFromSplited == true);
		CHECK(getHeader(f)->bFromSplited == true);
		CHECK(getHeader(f_1)->bFromSplited == true);
		CHECK(getHeader(f_2)->bFromSplited == true);
		CHECK(getHeader(f_3)->bFromSplited == true);
		CHECK(getHeader(f_4)->bFromSplited == true);

	}
	
	
	/*
	 * 04/01/2024
	 *
	 * Create an allocator that merges memory when we give memory back to the allocator, so we can reuse that block for future allocations
	 * bigger or smaller.
	 * */


	struct Chunk
	{
		// OPT
		size_t size = 0;
		Chunk* prev = nullptr;
		Chunk* next = nullptr;
		uint8 flags = 0x0000;
		uint8 _pad[7];
		word_t data[1];
	};

	enum class EChunkFlags : uint8
	{
		FROM_SPPLITED = 1 << 0,
		USED = 1 << 1 		
	};

	namespace helper
	{
		size_t align(size_t size, size_t alingment)
		{
			return (size + alingment - 1) & ~(alingment - 1);
		}

		Chunk* getHeaderChunk(void* ptr)
		{
			if(!ptr)
			{
				return nullptr;
			}

			const auto headerSize{ sizeof(Chunk) - sizeof(std::declval<Chunk>().data) };
			auto chunk =  reinterpret_cast<Chunk*>(reinterpret_cast<char*>(ptr) - headerSize);
			return chunk;
		}

		size_t getMetaDataSize()
		{
			return sizeof(std::declval<Chunk>().data);
		}

		size_t getHeaderSize()
		{
			return sizeof(Chunk) - getMetaDataSize();
		}

		void SetUsed(Chunk* chunk, bool value)
		{
			if(!chunk)
			{
				return;
			}

			if(value)
			{
				chunk->flags |= (uint8)(EChunkFlags::USED);
			}
			else
			{
				chunk->flags &= (~(uint8)(EChunkFlags::USED));
			}

		}

		bool IsUsed(Chunk* chunk)
		{
			if(!chunk)
			{
				return false;
			}

			return chunk->flags & (uint8)(EChunkFlags::USED);
		}
	}


	struct MergingAllocator
	{
		MergingAllocator() = default;
		MergingAllocator(char* buffer, size_t memoryMax)
			: m_buffer{ buffer }
			, m_memoryMax{ memoryMax }
		{
		}

		void* Allocate(size_t sizeToAllocate, size_t alingment = 8)
		{
			const auto size = (sizeToAllocate + alingment - 1) & ~(alingment - 1);
			const auto headerS = helper::getHeaderSize();
			
			auto it = m_LastFreeChunk;
			auto bookMark = it;

			// explore fwd
			while(it != nullptr)
			{
				if(it->size == size)
				{
					return it->data;
				}
				else if(it->size >= size)
				{
					// 1					
					if(it->size - size >= sizeof(Chunk))
					{
						const auto splittedBlockSize = it->size - (headerS+ size);
						Chunk* splittedChunk = (Chunk*)(reinterpret_cast<char*>(it->data) + size);
						splittedChunk->size = splittedBlockSize;

						// link
						splittedChunk->prev = it;
						splittedChunk->next = it->next;
						it->next = splittedChunk;

						helper::SetUsed(it, true);
						return it->data;
					}

					//2
					if(it->size - (helper::getHeaderSize() + size) >= helper::getMetaDataSize())
					{

					}
				}

				it = it->next;
			}

			it = bookMark;
			while (it != nullptr)
			{
				if (it->size == size)
				{
					return it->data;
				}
				else if (it->size >= size)
				{
					// 1					
					if (it->size - size >= sizeof(Chunk))
					{
						const auto splittedBlockSize = it->size - (headerS+ size);
						Chunk* splittedChunk = (Chunk*)(reinterpret_cast<char*>(it->data) + size);
						splittedChunk->size = splittedBlockSize;

						// link
						splittedChunk->prev = it;
						splittedChunk->next = it->next;
						it->next = splittedChunk;

						helper::SetUsed(it, true);
						return it->data;
					}

					//2
					if (it->size - (helper::getHeaderSize() + size) >= helper::getMetaDataSize())
					{

					}
				}

				it = it->prev;
			}

			const auto memoryOnceAllocatedChunk = m_MemoryAllocated + (headerS + size);
			if(memoryOnceAllocatedChunk > m_memoryMax)
			{
				throw std::bad_alloc{ };
			}

			Chunk* newChunk = (Chunk*)(m_buffer + m_MemoryAllocated);
			newChunk->size = size;
			helper::SetUsed(newChunk, true);
			if(m_LastCreatedChunk)
			{
				m_LastCreatedChunk->next = newChunk;
				newChunk->prev = m_LastCreatedChunk;
				m_LastCreatedChunk->next = newChunk;
			}
			m_LastCreatedChunk = newChunk;
			m_MemoryAllocated = memoryOnceAllocatedChunk;

			return newChunk->data;
		}

		void Free(void* ptr)
		{
			if(!ptr)
			{
				return;
			}

			const auto headerS = helper::getHeaderSize();

			auto freedChunk = helper::getHeaderChunk(ptr);
			helper::SetUsed(freedChunk, false);
			m_LastFreeChunk = freedChunk;

			// merge fwd
			auto it = freedChunk;
			auto bookMark = it;
			while(it != nullptr)
			{
				auto next = it->next;
				if(next && !helper::IsUsed(next))
				{
					next->next->prev = it;
					it->next = next->next;
					it->size += (headerS + next->size);

					m_LastFreeChunk = it;
				}

				it = next;
			}

			it = bookMark;
			while (it != nullptr)
			{
				auto prev = it->prev;
				auto next = it->next;
				if (prev && !helper::IsUsed(prev))
				{
					prev->next = next;
					if(next)
					{
						next->prev = prev;
					}
					prev->size += (headerS + it->size);

					m_LastFreeChunk = prev;
				}

				it = prev;
			}

		}

		// chunks
		Chunk* m_LastFreeChunk = nullptr;
		Chunk* m_LastCreatedChunk = nullptr;

		// memory
		char* m_buffer = nullptr;
		size_t m_memoryMax = 0;
		size_t m_MemoryAllocated = 0;
	};


	TEST_CASE("Seg Allocator, OK")
	{
		auto size = Megabytes(10);
		char* buffer = (char*)(VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
		MergingAllocator alloc{buffer, size};
		int* a = (int*)alloc.Allocate(sizeof(a));
		*a = 4;
		CHECK(*a == 4);

		CHECK((helper::getHeaderChunk(a)->flags & (uint8)EChunkFlags::USED) > 0);

		assert(VirtualFree(buffer, 0, MEM_RELEASE) != 0);
	}

	TEST_CASE("Seg Allocator, Merging once freed, OK")
	{
		struct Dog_16
		{
			uint8 data[16];
		};

		auto size = Megabytes(10);
		char* buffer = (char*)(VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
		MergingAllocator alloc{ buffer, size };
		int* a = (int*)alloc.Allocate(sizeof(int), 8);
		*a = 4;
		alloc.Free(a);

		Dog_16* dog;
		dog = (Dog_16*)alloc.Allocate(sizeof(Dog_16), 8);

		alloc.Free(dog);
		auto block = helper::getHeaderChunk(a);
		auto fff = sizeof(int) + sizeof(Chunk) + sizeof(Dog_16);
		CHECK((helper::getHeaderChunk(a)->size == sizeof(std::declval<Chunk>().data) + helper::getHeaderSize() + sizeof(Dog_16)) == true);
		CHECK((helper::getHeaderChunk(a)->flags & (uint8)EChunkFlags::USED) == 0);
	}

	TEST_CASE("Merging Allocator, Out of memeory")
	{
		char* buffer = (char*)(VirtualAlloc(NULL, Byte(8), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
		MergingAllocator alloc{ buffer, Byte(8) };
		CHECK_THROWS(alloc.Allocate(sizeof(int), 8), std::bad_alloc{ });
	}

	std::map<struct Traceable*, struct ObjectHeader*> gTraceInfo;


	struct ObjectHeader
	{
		bool marked;
		size_t size;
	};

	struct Traceable
	{
		ObjectHeader* GetHeader() { return gTraceInfo.at(this); };

		static void* operator new(size_t size)
		{
			void* obj = ::operator new(size);

			auto header = new ObjectHeader{ false, size };
			gTraceInfo.insert(std::make_pair((Traceable*)obj, header));

			return obj;
		}

	};

	intptr_t* __rsp = nullptr;
	intptr_t* __stackBegin = nullptr;

	intptr_t* __rbp = nullptr;
	extern "C" uintptr_t read_rbp(intptr_t**);
	extern "C" uintptr_t read_rsp(intptr_t**);
	#define __READ_RBP() read_rbp(&__rbp) 
	#define __READ_RSP() read_rsp(&__rsp)


	void gcInit()
	{
		__READ_RBP();
		__stackBegin = (intptr_t*)*__rbp;
	}


	bool IsPointerValid(const void* ptr)
	{
		MEMORY_BASIC_INFORMATION mbi;
		if (VirtualQuery(ptr, &mbi, sizeof(mbi)))
		{
			bool isAccessible = mbi.State == MEM_COMMIT &&
				(mbi.Protect == PAGE_READONLY ||
					mbi.Protect == PAGE_READWRITE ||
					mbi.Protect == PAGE_EXECUTE_READ ||
					mbi.Protect == PAGE_EXECUTE_READWRITE);
			return isAccessible && mbi.AllocationProtect != PAGE_NOACCESS;
		}
		return false;
	}

	/**
	 * Traverses the stacks to obtain the roots.
	 */
	std::vector<Traceable*> getRoots() 
	{
		std::vector<Traceable*> result;


		__READ_RSP();
		auto rsp = (uint8_t*)__rsp;
		auto top = (uint8_t*)__stackBegin;


		while (rsp < (uint8*)__rbp) 
		{
			auto address = (Traceable*)*(uintptr_t*)rsp;
			if (gTraceInfo.count(address) != 0) 
			{
				result.emplace_back(address);
			}
			rsp++;
		}

		return result;
	}


	std::vector<Traceable*> getPointers(Traceable* obj)
	{
		auto p = (uint8*)obj;
		auto end = (p + obj->GetHeader()->size);
		std::vector<Traceable*> result;
		while(p < end)
		{
			auto address = (Traceable*)*(uintptr_t*)p;
			if(gTraceInfo.count(address) != 0)
			{
				result.emplace_back(address);
			}

			p++;
		}

		return result;
	}

	struct Node : public Traceable
	{
		Node() = default;
		Node(char nameParam, Node* leftParam = nullptr, Node* rightParam = nullptr)
			: name{ nameParam }
			, left{ leftParam }
			, right{ rightParam }
		{

		}
		char name;
		Node* left = nullptr;
		Node* right = nullptr;
	};

	void mark()
	{
		auto worklist = getRoots();
		while(!worklist.empty())
		{
			auto o = worklist.back();
			worklist.pop_back();
			auto header = o->GetHeader();

			if(!header->marked)
			{
				header->marked = true;
				for(const auto& p : getPointers(o))
				{
					worklist.push_back(p);
				}
			}

		}
	}

	void sweep()
	{
		auto it = gTraceInfo.cbegin();
		while(it != gTraceInfo.cend())
		{
			if(it->second->marked)
			{
				it->second->marked = false;
				++it;
			}
			else
			{
				it = gTraceInfo.erase(it);
				delete it->first;
			}
		}
	}

	void gc()
	{
		mark();
		sweep();
	}

	TEST_CASE("Garbage Collector Test")
	{
	}

}
