
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

}
