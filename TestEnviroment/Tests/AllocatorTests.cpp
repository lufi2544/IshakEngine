#include "doctest.h"
#include "Core/Memory/Allocator.h"
#include "Core/Memory/MemoryManager.h"
#include "Core/Memory/Memory.h"


namespace ishak::tests {


	namespace helper
	{
		bool IsMemoryEqual(void* ptr1, void* ptr2)
		{
			return (uint64)(ptr1) == (uint64)(ptr2);
		}
	}



	TEST_CASE("BestFitAllocator, NO Memory leak, OK")
	{
		Memory::MemoryManager manager{ Byte(1000) };
		Memory::MemoryManager::SandBox* sandBox = manager.GetMemorySandBox(Memory::EMemoryHirarchy::PERSISTENT, Memory::EMemorySandBoxType::CORE);
		const size_t systemMemory = Byte(100);
		void* rawMemory = sandBox->Allocate(systemMemory);

		Memory::BestFitFreeAllocator alloc{ Memory::FMemoryChunk{ rawMemory, systemMemory } };
		auto Memory1 = alloc.Allocate(8);
		auto Memory2 = alloc.Allocate(8);
		alloc.Free(Memory1);
		alloc.Free(Memory2);

		CHECK(alloc.GetLeaks() == 0);
	}


	TEST_CASE("BestFitAllocator, Memory leak, OK")
	{
		Memory::MemoryManager manager{ Byte(1000) };
		Memory::MemoryManager::SandBox* sandBox = manager.GetMemorySandBox(Memory::EMemoryHirarchy::PERSISTENT, Memory::EMemorySandBoxType::CORE);
		const size_t systemMemory = Byte(100);
		void* rawMemory = sandBox->Allocate(systemMemory);
		Memory::BestFitFreeAllocator alloc{ Memory::FMemoryChunk{ rawMemory, systemMemory } };
		auto Memory1 = alloc.Allocate(8);
		auto Memory2 = alloc.Allocate(8);

		CHECK(alloc.GetLeaks() == 2);

		alloc.Free(Memory1);
		alloc.Free(Memory2);
	}

	TEST_CASE("IshakAllocator, allocator from sandBox Memory, create an obj, OK")
	{
		struct Foo
		{
			int a, b;
			char letter;
		};

		typedef TArray<Foo, Memory::InlineAllocator<Foo, 5, 4>> IdsT;
		IdsT ids;

		Memory::MemoryManager alloc{ Byte(1000) };
		Memory::MemoryManager::SandBox* sandBox = alloc.GetMemorySandBox(Memory::EMemoryHirarchy::PERSISTENT, Memory::EMemorySandBoxType::CORE);
		const size_t systemMemory{ (IdsT::AllocatorType::GetNeededMemory()) };
		void* rawMemory = sandBox->Allocate(IdsT::AllocatorType::GetNeededMemory());
		IdsT::AllocatorType inlineAlloc{ Memory::FMemoryChunk{ rawMemory, systemMemory} };

		ids = IdsT(inlineAlloc);

		ids.Add(Foo{1, 1, '1'});
		ids.Add(Foo{2, 2, '2'});
		ids.Add(Foo{3, 3, '3'});
		ids.Add(Foo{4, 4, '4'});

		ids[0].a = 44;

		CHECK(ids[0].a == 44);
	}

	TEST_CASE("InlineAllocator, added to array, OK")
	{
		Memory::MemoryManager alloc{ Byte(1000) };
		Memory::MemoryManager::SandBox* sandBox = alloc.GetMemorySandBox(Memory::EMemoryHirarchy::PERSISTENT, Memory::EMemorySandBoxType::CORE);
		const size_t systemMemory = Byte(112);
		void* rawMemory = sandBox->Allocate(systemMemory);
		Memory::InlineAllocator<int, 4, 4> allocator{ Memory::FMemoryChunk{ rawMemory, systemMemory} };
		TArray<int, Memory::InlineAllocator<int, 4, 4>> ids{allocator};

		ids.Add(1);
		ids.Add(2);
		ids.Add(3);
		ids.Add(4);

		CHECK(ids[0] == 1);
		CHECK(ids[1] == 2);
		CHECK(ids[2] == 3);
		CHECK(ids[3] == 4);
	}

	TEST_CASE("InlineAllocator, request more Memory than allocated, EXCEPTION")
	{
		struct Foo
		{
			int a, b;
			char letter;
		};

		Memory::MemoryManager alloc{ Byte(1000) };
		Memory::MemoryManager::SandBox* sandBox = alloc.GetMemorySandBox(Memory::EMemoryHirarchy::PERSISTENT, Memory::EMemorySandBoxType::CORE);
		const size_t systemMemory = Byte(112);
		void* rawMemory = sandBox->Allocate(systemMemory);
		Memory::InlineAllocator<int, 4> allocator{ Memory::FMemoryChunk{ rawMemory, systemMemory} };
		TArray<int, Memory::InlineAllocator<int, 4>> ids{allocator };

		ids.Add(1);
		ids.Add(2);
		ids.Add(3);
		ids.Add(4);

		CHECK(ids[0] == 1);
		CHECK(ids[1] == 2);
		CHECK(ids[2] == 3);
		CHECK(ids[3] == 4);
		CHECK_THROWS(ids.Add(5), std::bad_alloc{ });
	}

	TEST_CASE("Inline Allocator, OK")
	{
		Memory::MemoryManager alloc{ Byte(1000) };
		Memory::MemoryManager::SandBox* sandBox = alloc.GetMemorySandBox(Memory::EMemoryHirarchy::PERSISTENT, Memory::EMemorySandBoxType::CORE);
		const size_t systemMemory = Byte(112);
		void* rawMemory = sandBox->Allocate(systemMemory);
		Memory::InlineAllocator<int, 4> allocator{ Memory::FMemoryChunk{ rawMemory, systemMemory} };

		int* datas{ (int*)allocator.Allocate(sizeof(int) * 4)};
		datas[0] = 0;
		datas[1] = 1;
		datas[2] = 2;
		datas[3] = 3;
		CHECK(datas[3] == 3);

	}

	TEST_CASE("FixedSize, requested Memory, modified object, OK")
	{
		struct Foo
		{
			int a, b;
			char letter;
		};

		Memory::MemoryManager alloc{ Byte(1000) };
		Memory::MemoryManager::SandBox* sandBox = alloc.GetMemorySandBox(Memory::EMemoryHirarchy::PERSISTENT, Memory::EMemorySandBoxType::CORE);
		const size_t systemMemory = Byte(112);
		void* rawMemory = sandBox->Allocate(systemMemory);
		Memory::FixedSizeAllocator<Foo, 4, 4> allocator{ Memory::FMemoryChunk{ rawMemory, systemMemory} };

		Foo* ptr = (Foo*)allocator.Allocate();
		ptr->a = 1;
		ptr->b = 2;
		ptr->letter = 's';

		CHECK(ptr != nullptr);
	}

	TEST_CASE("FixedSize, requested Memory, no enough Memory, OK")
	{
		Memory::MemoryManager alloc{ Byte(1000)};
		Memory::MemoryManager::SandBox* sandBox = alloc.GetMemorySandBox(Memory::EMemoryHirarchy::PERSISTENT, Memory::EMemorySandBoxType::CORE);
		void* rawMemory = sandBox->Allocate(Byte(10));
		Memory::FixedSizeAllocator<int, 4, 4> allocator{ Memory::FMemoryChunk{ rawMemory, Byte(10)} };

		int* ptr = (int*)allocator.Allocate();
		CHECK(ptr == nullptr);
	}

	TEST_CASE("FixSizedAllocator, created Chunks, OK")
	{
		Memory::MemoryManager alloc{ MemoryPages(1) };
		Memory::MemoryManager::SandBox* sandBox = alloc.GetMemorySandBox(Memory::EMemoryHirarchy::PERSISTENT, Memory::EMemorySandBoxType::CORE);
		void* rawMemory = sandBox->Allocate(Byte(100));
		Memory::FixedSizeAllocator<int, 4, 4> allocator{ Memory::FMemoryChunk{ rawMemory, Byte(100)} };

		int* ptr = (int*)allocator.Allocate();
		CHECK(ptr != nullptr);
	}

	TEST_CASE("IshakAllocator, request some Memory, OK")
	{
		struct FooSystem
		{
			FooSystem() = default;

			int data{4};
			float staticData{5.0f};
			bool bVal{ true };
		};
		Memory::MemoryManager alloc{ MemoryPages(1) };
		Memory::MemoryManager::SandBox* sandBox = alloc.GetMemorySandBox(Memory::EMemoryHirarchy::PERSISTENT, Memory::EMemorySandBoxType::CORE);	

		void* rawMemory = sandBox->Allocate(Byte(100));
		Memory::BestFitFreeAllocator systemAlloc{ Memory::FMemoryChunk{ rawMemory, Byte(100) }};

		TArray<int, Memory::BestFitFreeAllocator> container{ systemAlloc };

		container.Add(22);
		container.Add(22);
		container.Add(22);
		container.Add(22);
		container.Add(22);
		container.Add(22);

	}

	TEST_CASE("Ishak Allocator, Allocate Memory, Sandboxes, OK")
	{
		const size_t sizeToAlloc{ MemoryPages(1) };
		Memory::MemoryManager alloc{ sizeToAlloc };

		const int hirarchyMem = sizeToAlloc / (uint8)(Memory::EMemoryHirarchy::NUM);
		const int sandBoxMem = hirarchyMem / (uint8)(Memory::EMemorySandBoxType::NUM);

		bool bAllOk{ false };

		for(uint8 i = 0; i < (uint8)Memory::EMemoryHirarchy::NUM; ++i)
		{
			for(uint8 j = 0; j < (uint8)Memory::EMemorySandBoxType::NUM; ++j)
			{
				const Memory::MemoryManager::SandBox* sandbox{
					alloc.GetMemorySandBox((Memory::EMemoryHirarchy)i, (Memory::EMemorySandBoxType)j) };

				bAllOk = sandbox->size == sandBoxMem;

				if(!bAllOk)
				{
					break;
				}
			}
			if(!bAllOk)
			{
				break;
			}
		}

		CHECK(bAllOk == true);
	}

	TEST_CASE("Align tests")
	{
		auto a_8 = Memory::align(3);
		auto a_16 = Memory::align(11);
		auto a_24 = Memory::align(20);
		auto a_32 = Memory::align(30);
		auto a_40 = Memory::align(34);
		auto a_48 = Memory::align(46);
		auto a_56 = Memory::align(52);
		auto a_64 = Memory::align(60);

		CHECK(a_8 == 8);
		CHECK(a_16 == 16);
		CHECK(a_24 == 24);
		CHECK(a_32 == 32);
		CHECK(a_40 == 40);
		CHECK(a_48 == 48);
		CHECK(a_56 == 56);
		CHECK(a_64 == 64);
	}

	TEST_CASE("Defalt Allocator Test_ 8, OK")
	{
		Memory::FirstFitAllocator alloc;

		auto p1 = alloc.Allocate(3);
		auto header = Memory::helper::getHeader((word_t*)p1);
		CHECK(header->size == sizeof(word_t));
		CHECK(Memory::IsBlockInUse(header) == true);
	}

	TEST_CASE("Default Allocator TEST 16, OK")
	{
		Memory::FirstFitAllocator alloc;
		auto p1{ alloc.Allocate(11) };
		auto header{ Memory::helper::getHeader((word_t*)p1) };
		CHECK(header->size == 16);
		CHECK(Memory::IsBlockInUse(header)== true);
	}

	TEST_CASE("Default Allocator Free")
	{
		Memory::FirstFitAllocator alloc;
		word_t address;
		auto p{ alloc.Allocate(8) };
		auto header{ Memory::helper::getHeader((word_t*)p) };

		alloc.Free(p);
		CHECK(Memory::IsBlockInUse(header)== false);
	}


	TEST_CASE("Reused a Block, OK")
	{
		Memory::FirstFitAllocator alloc;
		void* p{ alloc.Allocate(8) };
		Memory::Block* header{ Memory::helper::getHeader((word_t*)p) };
		alloc.Free(p);

		uint64 add1 = (uint64)(p);

		void* p2{ alloc.Allocate(8) };
		CHECK(helper::IsMemoryEqual(p, p2) == true);

		alloc.Free(p2);
	}


	TEST_CASE("Next Fit Test, OK")
	{
		Memory::NextFitAllocator alloc;

		alloc.Allocate(8);
		alloc.Allocate(8);
		alloc.Allocate(8);
		auto a = alloc.Allocate(16);
		auto b = alloc.Allocate(16);
		auto c = alloc.Allocate(16);
		alloc.Allocate(8);
		alloc.Allocate(8);
		alloc.Allocate(8);

		alloc.Free(c);

		auto a_1 = alloc.Allocate(16);
		CHECK(c == a_1);

		alloc.Free(a);
		auto a_2 = alloc.Allocate(16);

		CHECK(a_2 == a);
		CHECK(Memory::helper::getHeader((word_t*)a_2) == alloc.m_lastAllocatedBlock);
	}

	TEST_CASE("Next fit, freed some blocks, when allocating, starting from the end, we should start from the begining when not found, OK")
	{
		Memory::NextFitAllocator alloc;

		alloc.Allocate(16);
		auto a = alloc.Allocate(8); 
		auto b = alloc.Allocate(16);
		alloc.Allocate(8); 

		alloc.Free(a);
		alloc.Free(b);

		auto c = alloc.Allocate(16);
		CHECK(c == b);
	}


	TEST_CASE("Best fit, Allocate, OK")
	{
		Memory::BestFitAllocator alloc;
		alloc.Allocate(8);
		auto a = alloc.Allocate(64);
		alloc.Allocate(8);
		auto b = alloc.Allocate(16);

		alloc.Free(b);
		alloc.Free(a);

		auto c = alloc.Allocate(16);
		CHECK(Memory::helper::getHeader((word_t*)b) == Memory::helper::getHeader((word_t*)c));

		// Spliting here
		auto d = alloc.Allocate(16);
		CHECK(Memory::helper::getHeader((word_t*)d) == Memory::helper::getHeader((word_t*)a));
	}


	TEST_CASE("Best fit, when deallocated, free blocks can merge, OK") 
	{
		Memory::BestFitAllocator alloc;

		auto a = alloc.Allocate(16);
		auto b = alloc.Allocate(16);

		alloc.Free(b);
		alloc.Free(a);

		auto c = alloc.Allocate(8);

		CHECK(Memory::helper::getHeader((word_t*)c) == Memory::helper::getHeader((word_t*)a));
	}


	TEST_CASE("Best Fit, deallocated, Merge with prev, OK")
	{
		Memory::BestFitFreeAllocator alloc;

		auto a = alloc.Allocate(32);
		auto b = alloc.Allocate(32);

		alloc.Free(a);
		alloc.Free(b);

		auto c = alloc.Allocate(8);

		CHECK(alloc.getHeader((word_t*)c) == alloc.getHeader((word_t*)b));
	}

	TEST_CASE("Best Fit, deallocated, try to allocate, current block greater than requested size, but sizeof(block + size) does not match current block, OK") 
	{
		Memory::BestFitFreeAllocator alloc;

		auto a = alloc.Allocate(16);
		auto b = alloc.Allocate(16);
		auto c = alloc.Allocate(8);

		alloc.Free(a);
		alloc.Free(c);
		alloc.Free(b);

		auto d = alloc.Allocate(8);

		CHECK(alloc.getHeader((word_t*)d) == alloc.getHeader((word_t*)c));
	}


	TEST_CASE("Best Fit, Allocated From Arena, OK")
	{
		size_t toAlloc = MemoryPages(1);
		void* Memory = VirtualAlloc(
			NULL,
			toAlloc,
			MEM_COMMIT | MEM_RESERVE,
			PAGE_READWRITE);


		Memory::BestFitFreeAllocator alloc = 
			Memory::BestFitFreeAllocator(Memory::FMemoryChunk{ Memory, toAlloc });

		auto a = alloc.Allocate(16);
		auto b = alloc.Allocate(16);
		auto c = alloc.Allocate(8);

		alloc.Free(a);
		alloc.Free(c);
		alloc.Free(b);

		auto d = alloc.Allocate(8);

		CHECK(alloc.getHeader((word_t*)d) == alloc.getHeader((word_t*)c));
		CHECK(VirtualFree(Memory, 0, MEM_RELEASE) != 0);
	}

#pragma region TEST

	TEST_CASE("Allocate something larger than the cache line")
	{
		
	}

	TEST_CASE("Allocates, out of Memory, exception.")
	{
		
	}
	TEST_CASE("Allocated Memory, size bigger than Chunk's, OK ")
	{
		// We should have not the first Memory block occupied only, but the first 2;
	}

	TEST_CASE("Get Memory from the OS and then allocate the Global Allocator there, Memory left counts with size minus the allocator's")
	{
		
	}


	TEST_CASE("Buffered Allocator")
	{


	}



#pragma endregion TEST

	//////////////////////////////




}// ishak::Memory
