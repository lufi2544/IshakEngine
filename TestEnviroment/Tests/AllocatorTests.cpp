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


	TEST_CASE("BestFitAllocator, memory leak, OK")
	{
		memory::MemoryManager manager{ Byte(1000) };
		memory::MemoryManager::SandBox* sandBox = manager.GetMemorySandBox(memory::EMemoryHirarchy::PERSISTENT, memory::EMemorySandBoxType::CORE);
		const size_t systemMemory = Byte(100);
		void* rawMemory = sandBox->Allocate(systemMemory);
		memory::BestFitFreeAllocator alloc{ memory::FMemoryChunk{ rawMemory, systemMemory } };
		auto memory1 = alloc.Allocate(8);
		auto memory2 = alloc.Allocate(8);

		CHECK(alloc.GetLeaks() == 2);

		alloc.Free(memory1);
		alloc.Free(memory2);
	}

	TEST_CASE("IshakAllocator, allocator from sandBox memory, create an obj, OK")
	{
		struct Foo
		{
			int a, b;
			char letter;
		};


		memory::MemoryManager alloc{ Byte(1000) };
		memory::MemoryManager::SandBox* sandBox = alloc.GetMemorySandBox(memory::EMemoryHirarchy::PERSISTENT, memory::EMemorySandBoxType::CORE);
		const size_t systemMemory = Byte(112);
		void* rawMemory = sandBox->Allocate(systemMemory);
		memory::InlineAllocator<Foo, 4, 4> allocator{ memory::FMemoryChunk{ rawMemory, systemMemory} };
		TArray<Foo, memory::InlineAllocator<Foo, 4, 4>> ids{ 4, allocator };

		ids.Add(Foo{1, 1, '1'});
		ids.Add(Foo{2, 2, '2'});
		ids.Add(Foo{3, 3, '3'});
		ids.Add(Foo{4, 4, '4'});

		ids[0].a = 44;

		CHECK(ids[0].a == 44);
	}

	TEST_CASE("InlineAllocator, added to array, OK")
	{
		memory::MemoryManager alloc{ Byte(1000) };
		memory::MemoryManager::SandBox* sandBox = alloc.GetMemorySandBox(memory::EMemoryHirarchy::PERSISTENT, memory::EMemorySandBoxType::CORE);
		const size_t systemMemory = Byte(112);
		void* rawMemory = sandBox->Allocate(systemMemory);
		memory::InlineAllocator<int, 4, 4> allocator{ memory::FMemoryChunk{ rawMemory, systemMemory} };
		TArray<int, memory::InlineAllocator<int, 4, 4>> ids{4, allocator};

		ids.Add(1);
		ids.Add(2);
		ids.Add(3);
		ids.Add(4);

		CHECK(ids[0] == 1);
		CHECK(ids[1] == 2);
		CHECK(ids[2] == 3);
		CHECK(ids[3] == 4);
	}

	TEST_CASE("InlineAllocator, request more memory than allocated, EXCEPTION")
	{
		struct Foo
		{
			int a, b;
			char letter;
		};

		memory::MemoryManager alloc{ Byte(1000) };
		memory::MemoryManager::SandBox* sandBox = alloc.GetMemorySandBox(memory::EMemoryHirarchy::PERSISTENT, memory::EMemorySandBoxType::CORE);
		const size_t systemMemory = Byte(112);
		void* rawMemory = sandBox->Allocate(systemMemory);
		memory::InlineAllocator<int, 4> allocator{ memory::FMemoryChunk{ rawMemory, systemMemory} };
		TArray<int, memory::InlineAllocator<int, 4>> ids{ 4, allocator };

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
		memory::MemoryManager alloc{ Byte(1000) };
		memory::MemoryManager::SandBox* sandBox = alloc.GetMemorySandBox(memory::EMemoryHirarchy::PERSISTENT, memory::EMemorySandBoxType::CORE);
		const size_t systemMemory = Byte(112);
		void* rawMemory = sandBox->Allocate(systemMemory);
		memory::InlineAllocator<int, 4> allocator{ memory::FMemoryChunk{ rawMemory, systemMemory} };

		int* datas{ (int*)allocator.Allocate(sizeof(int) * 4)};
		datas[0] = 0;
		datas[1] = 1;
		datas[2] = 2;
		datas[3] = 3;
		CHECK(datas[3] == 3);

	}

	TEST_CASE("FixedSize, requested memory, modified object, OK")
	{
		struct Foo
		{
			int a, b;
			char letter;
		};

		memory::MemoryManager alloc{ Byte(1000) };
		memory::MemoryManager::SandBox* sandBox = alloc.GetMemorySandBox(memory::EMemoryHirarchy::PERSISTENT, memory::EMemorySandBoxType::CORE);
		const size_t systemMemory = Byte(112);
		void* rawMemory = sandBox->Allocate(systemMemory);
		memory::FixedSizeAllocator<Foo, 4, 4> allocator{ memory::FMemoryChunk{ rawMemory, systemMemory} };

		Foo* ptr = (Foo*)allocator.Allocate();
		ptr->a = 1;
		ptr->b = 2;
		ptr->letter = 's';

		CHECK(ptr != nullptr);
	}

	TEST_CASE("FixedSize, requested memory, no enough memory, OK")
	{
		memory::MemoryManager alloc{ Byte(1000)};
		memory::MemoryManager::SandBox* sandBox = alloc.GetMemorySandBox(memory::EMemoryHirarchy::PERSISTENT, memory::EMemorySandBoxType::CORE);
		void* rawMemory = sandBox->Allocate(Byte(10));
		memory::FixedSizeAllocator<int, 4, 4> allocator{ memory::FMemoryChunk{ rawMemory, Byte(10)} };

		int* ptr = (int*)allocator.Allocate();
		CHECK(ptr == nullptr);
	}

	TEST_CASE("FixSizedAllocator, created Chunks, OK")
	{
		memory::MemoryManager alloc{ MemoryPages(1) };
		memory::MemoryManager::SandBox* sandBox = alloc.GetMemorySandBox(memory::EMemoryHirarchy::PERSISTENT, memory::EMemorySandBoxType::CORE);
		void* rawMemory = sandBox->Allocate(Byte(100));
		memory::FixedSizeAllocator<int, 4, 4> allocator{ memory::FMemoryChunk{ rawMemory, Byte(100)} };

		int* ptr = (int*)allocator.Allocate();
		CHECK(ptr != nullptr);
	}

	TEST_CASE("IshakAllocator, request some memory, OK")
	{
		struct FooSystem
		{
			FooSystem() = default;

			int data{4};
			float staticData{5.0f};
			bool bVal{ true };
		};
		memory::MemoryManager alloc{ MemoryPages(1) };
		memory::MemoryManager::SandBox* sandBox = alloc.GetMemorySandBox(memory::EMemoryHirarchy::PERSISTENT, memory::EMemorySandBoxType::CORE);	

		void* rawMemory = sandBox->Allocate(Byte(100));
		memory::BestFitFreeAllocator systemAlloc{ memory::FMemoryChunk{ rawMemory, Byte(100) }};

		//TODO allocator copy,  allocator constructor
		//alingment
		TArray<int, memory::BestFitFreeAllocator> container{ systemAlloc };

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
		memory::MemoryManager alloc{ sizeToAlloc };

		const int hirarchyMem = sizeToAlloc / (uint8)(memory::EMemoryHirarchy::NUM);
		const int sandBoxMem = hirarchyMem / (uint8)(memory::EMemorySandBoxType::NUM);

		bool bAllOk{ false };

		for(uint8 i = 0; i < (uint8)memory::EMemoryHirarchy::NUM; ++i)
		{
			for(uint8 j = 0; j < (uint8)memory::EMemorySandBoxType::NUM; ++j)
			{
				const memory::MemoryManager::SandBox* sandbox{
					alloc.GetMemorySandBox((memory::EMemoryHirarchy)i, (memory::EMemorySandBoxType)j) };

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
		auto a_8 = memory::align(3);
		auto a_16 = memory::align(11);
		auto a_24 = memory::align(20);
		auto a_32 = memory::align(30);
		auto a_40 = memory::align(34);
		auto a_48 = memory::align(46);
		auto a_56 = memory::align(52);
		auto a_64 = memory::align(60);

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
		memory::FirstFitAllocator alloc;

		auto p1 = alloc.Allocate(3);
		auto header = memory::helper::getHeader((word_t*)p1);
		CHECK(header->size == sizeof(word_t));
		CHECK(memory::IsBlockInUse(header) == true);
	}

	TEST_CASE("Default Allocator TEST 16, OK")
	{
		memory::FirstFitAllocator alloc;
		auto p1{ alloc.Allocate(11) };
		auto header{ memory::helper::getHeader((word_t*)p1) };
		CHECK(header->size == 16);
		CHECK(memory::IsBlockInUse(header)== true);
	}

	TEST_CASE("Default Allocator Free")
	{
		memory::FirstFitAllocator alloc;
		word_t address;
		auto p{ alloc.Allocate(8) };
		auto header{ memory::helper::getHeader((word_t*)p) };

		alloc.Free(p);
		CHECK(memory::IsBlockInUse(header)== false);
	}


	TEST_CASE("Reused a Block, OK")
	{
		memory::FirstFitAllocator alloc;
		void* p{ alloc.Allocate(8) };
		memory::Block* header{ memory::helper::getHeader((word_t*)p) };
		alloc.Free(p);

		uint64 add1 = (uint64)(p);

		void* p2{ alloc.Allocate(8) };
		CHECK(helper::IsMemoryEqual(p, p2) == true);

		alloc.Free(p2);
	}


	TEST_CASE("Next Fit Test, OK")
	{
		memory::NextFitAllocator alloc;

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
		CHECK(memory::helper::getHeader((word_t*)a_2) == alloc.m_lastAllocatedBlock);
	}

	TEST_CASE("Next fit, freed some blocks, when allocating, starting from the end, we should start from the begining when not found, OK")
	{
		memory::NextFitAllocator alloc;

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
		memory::BestFitAllocator alloc;
		alloc.Allocate(8);
		auto a = alloc.Allocate(64);
		alloc.Allocate(8);
		auto b = alloc.Allocate(16);

		alloc.Free(b);
		alloc.Free(a);

		auto c = alloc.Allocate(16);
		CHECK(memory::helper::getHeader((word_t*)b) == memory::helper::getHeader((word_t*)c));

		// Spliting here
		auto d = alloc.Allocate(16);
		CHECK(memory::helper::getHeader((word_t*)d) == memory::helper::getHeader((word_t*)a));
	}


	TEST_CASE("Best fit, when deallocated, free blocks can merge, OK") 
	{
		memory::BestFitAllocator alloc;

		auto a = alloc.Allocate(16);
		auto b = alloc.Allocate(16);

		alloc.Free(b);
		alloc.Free(a);

		auto c = alloc.Allocate(8);

		CHECK(memory::helper::getHeader((word_t*)c) == memory::helper::getHeader((word_t*)a));
	}


	TEST_CASE("Best Fit, deallocated, Merge with prev, OK")
	{
		memory::BestFitFreeAllocator alloc;

		auto a = alloc.Allocate(32);
		auto b = alloc.Allocate(32);

		alloc.Free(a);
		alloc.Free(b);

		auto c = alloc.Allocate(8);

		CHECK(alloc.getHeader((word_t*)c) == alloc.getHeader((word_t*)b));
	}

	TEST_CASE("Best Fit, deallocated, try to allocate, current block greater than requested size, but sizeof(block + size) does not match current block, OK") 
	{
		memory::BestFitFreeAllocator alloc;

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
		void* memory = VirtualAlloc(
			NULL,
			toAlloc,
			MEM_COMMIT | MEM_RESERVE,
			PAGE_READWRITE);


		memory::BestFitFreeAllocator alloc = 
			memory::BestFitFreeAllocator(memory::FMemoryChunk{ memory, toAlloc });

		auto a = alloc.Allocate(16);
		auto b = alloc.Allocate(16);
		auto c = alloc.Allocate(8);

		alloc.Free(a);
		alloc.Free(c);
		alloc.Free(b);

		auto d = alloc.Allocate(8);

		CHECK(alloc.getHeader((word_t*)d) == alloc.getHeader((word_t*)c));
		CHECK(VirtualFree(memory, 0, MEM_RELEASE) != 0);
	}

#pragma region TEST
	/*
	TEST_CASE("Allocated the whole engine memory, OK")
	{
		MemoryManager& memManger = MemoryManager::Get();
		EngineMemory const* memory = memManger.GetMemory();

		CHECK(memory->PermanentMemory != NULL);
		CHECK(memory->TransientMemory != NULL);
	}

	TEST_CASE("Engine Allocator Created, MemoryBlocks created, OK")
	{

		MemoryManager& memManger = MemoryManager::Get();
		EngineMemory * memory = memManger.GetMemory();
		FPoolAllocator a{ memory,8  };

		char* j;
		j = reinterpret_cast<char*>(a.Malloc(16));

		j[0] = 'H';
		j[1] = 'o';

		a.Free(j);


	}

*/

	/*
	TEST_CASE("Engine Allocator, allocate stuff, memory aligned, less than cache line, allocated.")
	{
		struct Dog
		{
			int id;
		};

		auto& mem_manager = MemoryManager::Get();
		FEngineMalloc* engineMalloc = new FEngineMalloc(*mem_manager.GetMemory());

		Dog* allocated = new (engineMalloc->Malloc(sizeof(Dog))) (Dog);

		allocated->id = 3;
		delete engineMalloc;

		CHECK((allocated->id == 3));
	}

	TEST_CASE("Allocate size equal to the cacheLine size, OK")
	{

		struct Dog_8
		{
			int a;
			int b;
		};

		auto& mem_manager = MemoryManager::Get();
		FEngineMalloc* engineMalloc = new FEngineMalloc(*mem_manager.GetMemory());


		Dog_8* obj = new (engineMalloc->Malloc(sizeof(Dog_8))) (Dog_8);

		obj->a = 1;
		obj->b = 2;

		delete engineMalloc;

		CHECK(obj->a == 1);
		CHECK(obj->b == 2);
	}

	int GetAllocatedBitsInBlock(MemoryBlock* block)
	{
		int allocated = 0;
		if (block)
		{
			for (int j = 0; j < MemoryBlock::SIZE; ++j)
			{
				if (block->availableBytes.test(j))
				{
					++allocated;
				}
			}
		}

		return allocated;
	}

	bool IsBytesAvailable(MemoryBlock* block, int num)
	{
		if(block)
		{
			return std::find(block->contiguousAvailableBytes.begin(), block->contiguousAvailableBytes.end(), num) != std::end(block->contiguousAvailableBytes);
		}

		return false;
	}

	struct Dog_1
	{
		char a;
	};

	TEST_CASE("Allocate 1 byte obj")
	{
		auto& mem_manager = MemoryManager::Get();
		FEngineMalloc* engineMalloc = new FEngineMalloc(*mem_manager.GetMemory());


		MemoryBlock** blocks = new MemoryBlock*[5];
		size_t allocated;
		Dog_1* obj = new (engineMalloc->Malloc_Debug(sizeof(Dog_1), *blocks, allocated)) (Dog_1);

		obj->a = 'a';

		delete engineMalloc;

		CHECK(obj->a == 'a');
		CHECK(allocated == 1);

		// TODO FUNCTION
		// Check if the contiguous memory free is OK and the available bits.
		MemoryBlock* allocatedBlock{ blocks[0] };
		const int allocatedBytes = GetAllocatedBitsInBlock(allocatedBlock);

		CHECK(IsBytesAvailable(allocatedBlock, MemoryBlock::SIZE - sizeof(Dog_1)));
		CHECK(allocatedBytes == sizeof(Dog_1));

		delete blocks;
	}

	TEST_CASE("Free 1 byte")
	{

		auto& mem_manager = MemoryManager::Get();
		FEngineMalloc* engineMalloc = new FEngineMalloc(*mem_manager.GetMemory());


		MemoryBlock** blocks = new MemoryBlock * [5];
		size_t allocated;
		Dog_1* obj = new (engineMalloc->Malloc_Debug(sizeof(Dog_1), *blocks, allocated)) (Dog_1);
		engineMalloc->Free(obj);


		delete engineMalloc;
		delete blocks;
	}

	TEST_CASE("Allocate 2 byte obj")
	{
		struct Dog_2
		{
			char a;
			char b;
		};

		auto& mem_manager = MemoryManager::Get();
		FEngineMalloc* engineMalloc = new FEngineMalloc(*mem_manager.GetMemory());

		MemoryBlock** blocks = new MemoryBlock * [5];
		size_t allocatedNum = 0;
		Dog_2* obj = new (engineMalloc->Malloc_Debug(sizeof(Dog_2), *blocks, allocatedNum))(Dog_2);

		obj->a = 'a';
		obj->b = 'b';

		delete engineMalloc;

		CHECK(obj->a == 'a');
		CHECK(obj->b == 'b');
		// TODO Add MAlloc_Test(size, block*)-> void*

				// TODO FUNCTION
		// Check if the contiguous memory free is OK and the available bits.
		MemoryBlock* allocatedBlock{ blocks[0] };
		const int allocatedBytes = GetAllocatedBitsInBlock(allocatedBlock);

		CHECK(IsBytesAvailable(allocatedBlock, MemoryBlock::SIZE - sizeof(Dog_2)));
		CHECK(allocatedBytes == sizeof(Dog_2));

		delete blocks;
	}

	TEST_CASE("Allocate 3 byte obj")
	{
		struct Dog_3
		{
			char a;
			char b;
			char c;
		};

		auto& mem_manager = MemoryManager::Get();
		FEngineMalloc* engineMalloc = new FEngineMalloc(*mem_manager.GetMemory());

		MemoryBlock** blocks = new MemoryBlock * [5];
		size_t allocatedNum = 0;
		Dog_3* obj = new (engineMalloc->Malloc_Debug(sizeof(Dog_3), *blocks, allocatedNum))(Dog_3);

		obj->a = 'a';
		obj->b = 'b';
		obj->c = 'c';

		delete engineMalloc;

		CHECK(obj->a == 'a');
		CHECK(obj->b == 'b');
		CHECK(obj->c == 'c');

		MemoryBlock* allocatedBlock{ blocks[0] };
		const int allocatedBytes = GetAllocatedBitsInBlock(allocatedBlock);

		CHECK(IsBytesAvailable(allocatedBlock, MemoryBlock::SIZE - sizeof(Dog_3)));
		CHECK(allocatedBytes == sizeof(Dog_3));

		delete blocks;

	}

	TEST_CASE("Allocate 4 byte obj")
	{
		struct Dog_4
		{
			char a;
			char b;
			char c;
			char d;
		};

		auto& mem_manager = MemoryManager::Get();
		FEngineMalloc* engineMalloc = new FEngineMalloc(*mem_manager.GetMemory());

		MemoryBlock** blocks = new MemoryBlock * [5];
		size_t allocatedNum = 0;
		Dog_4* obj = new (engineMalloc->Malloc_Debug(sizeof(Dog_4), *blocks, allocatedNum))(Dog_4);

		obj->a = 'a';
		obj->b = 'b';
		obj->c = 'c';
		obj->d = 'd';

		delete engineMalloc;

		CHECK(obj->a == 'a');
		CHECK(obj->b == 'b');
		CHECK(obj->c == 'c');
		CHECK(obj->d == 'd');

		MemoryBlock* allocatedBlock{ blocks[0] };
		const int allocatedBytes = GetAllocatedBitsInBlock(allocatedBlock);

		CHECK(IsBytesAvailable(allocatedBlock, MemoryBlock::SIZE - sizeof(Dog_4)));
		CHECK(allocatedBytes == sizeof(Dog_4));

		delete blocks;
	}

	TEST_CASE("Allocate 5 byte obj")
	{
		struct Dog_5
		{
			char a;
			char b;
			char c;
			char d;
			char e;
		};

		auto& mem_manager = MemoryManager::Get();
		FEngineMalloc* engineMalloc = new FEngineMalloc(*mem_manager.GetMemory());

		MemoryBlock** blocks = new MemoryBlock * [5];
		size_t allocatedNum = 0;
		Dog_5* obj = new (engineMalloc->Malloc_Debug(sizeof(Dog_5), *blocks, allocatedNum))(Dog_5);

		obj->a = 'a';
		obj->b = 'b';
		obj->c = 'c';

		delete engineMalloc;

		CHECK(obj->a == 'a');
		CHECK(obj->b == 'b');
		CHECK(obj->c == 'c');

		MemoryBlock* allocatedBlock{ blocks[0] };
		const int allocatedBytes = GetAllocatedBitsInBlock(allocatedBlock);

		CHECK(IsBytesAvailable(allocatedBlock, MemoryBlock::SIZE - sizeof(Dog_5)));
		CHECK(allocatedBytes == sizeof(Dog_5));

		delete blocks;
	}

	TEST_CASE("Allocate 6 byte obj")
	{
		struct Dog_6
		{
			char a;
			char b;
			char c;
			char d;
			char e;
			char f;
		};

		auto& mem_manager = MemoryManager::Get();
		FEngineMalloc* engineMalloc = new FEngineMalloc(*mem_manager.GetMemory());

		MemoryBlock** blocks = new MemoryBlock * [5];
		size_t allocatedNum = 0;
		Dog_6* obj = new (engineMalloc->Malloc_Debug(sizeof(Dog_6), *blocks, allocatedNum))(Dog_6);

		obj->a = 'a';
		obj->b = 'b';
		obj->c = 'c';
		obj->d = 'd';
		obj->e = 'e';
		obj->f = 'f';

		delete engineMalloc;

		CHECK(obj->a == 'a');
		CHECK(obj->b == 'b');
		CHECK(obj->c == 'c');
		CHECK(obj->d == 'd');
		CHECK(obj->e == 'e');
		CHECK(obj->f == 'f');

		MemoryBlock* allocatedBlock{ blocks[0] };
		const int allocatedBytes = GetAllocatedBitsInBlock(allocatedBlock);

		CHECK(IsBytesAvailable(allocatedBlock, MemoryBlock::SIZE - sizeof(Dog_6)));
		CHECK(allocatedBytes == sizeof(Dog_6));

		delete blocks;
	}

	TEST_CASE("Allocate 7 byte obj")
	{
		struct Dog_7
		{
			char a;
			char b;
			char c;
			char d;
			char e;
			char f;
			char g;
		};

		auto& mem_manager = MemoryManager::Get();
		FEngineMalloc* engineMalloc = new FEngineMalloc(*mem_manager.GetMemory());

		MemoryBlock** blocks = new MemoryBlock * [5];
		size_t allocatedNum = 0;
		Dog_7* obj = new (engineMalloc->Malloc_Debug(sizeof(Dog_7), *blocks, allocatedNum))(Dog_7);

		obj->a = 'a';
		obj->b = 'b';
		obj->c = 'c';
		obj->d = 'd';
		obj->e = 'e';
		obj->f = 'f';
		obj->g = 'g';

		delete engineMalloc;

		CHECK(obj->a == 'a');
		CHECK(obj->b == 'b');
		CHECK(obj->c == 'c');
		CHECK(obj->d == 'd');
		CHECK(obj->e == 'e');
		CHECK(obj->f == 'f');
		CHECK(obj->g == 'g');

		MemoryBlock* allocatedBlock{ blocks[0] };
		const int allocatedBytes = GetAllocatedBitsInBlock(allocatedBlock);

		CHECK(IsBytesAvailable(allocatedBlock, MemoryBlock::SIZE - sizeof(Dog_7)));
		CHECK(allocatedBytes == sizeof(Dog_7));

		delete blocks;
	}

	TEST_CASE("Allocate 8 byte obj")
	{
		struct Dog_8
		{
			char a;
			char b;
			char c;
			char d;
			char e;
			char f;
			char g;
			char h;
		};

		auto& mem_manager = MemoryManager::Get();
		FEngineMalloc* engineMalloc = new FEngineMalloc(*mem_manager.GetMemory());

		MemoryBlock** blocks = new MemoryBlock * [5];
		size_t allocatedNum = 0;
		Dog_8* obj = new (engineMalloc->Malloc_Debug(sizeof(Dog_8), *blocks, allocatedNum))(Dog_8);

		obj->a = 'a';
		obj->b = 'b';
		obj->c = 'c';
		obj->d = 'd';
		obj->e = 'e';
		obj->f = 'f';
		obj->g = 'g';
		obj->h = 'h';

		delete engineMalloc;

		CHECK(obj->a == 'a');
		CHECK(obj->b == 'b');
		CHECK(obj->c == 'c');
		CHECK(obj->d == 'd');
		CHECK(obj->e == 'e');
		CHECK(obj->f == 'f');
		CHECK(obj->g == 'g');
		CHECK(obj->h == 'h');

		MemoryBlock* allocatedBlock{ blocks[0] };
		const int allocatedBytes = GetAllocatedBitsInBlock(allocatedBlock);

		CHECK(IsBytesAvailable(allocatedBlock, MemoryBlock::SIZE - sizeof(Dog_8)));
		CHECK(allocatedBytes == sizeof(Dog_8));

		delete blocks;
	}

	TEST_CASE("Allocate something larger than the cache line")
	{
		// We will allocate a Dog_9
		struct Dog_9
		{
			char a;
			char* b{ nullptr };
		};

		/*
		auto& mem_manager = MemoryManager::Get();
		FEngineMalloc* engineMalloc = new FEngineMalloc(*mem_manager.GetMemory());

		MemoryBlock** blocks = new MemoryBlock* [5];
		size_t allocatedNum = 0;
		Dog_9* obj = new (engineMalloc->Malloc_Debug(sizeof(Dog_9), *blocks, allocatedNum))(Dog_9);



		delete engineMalloc;
		delete blocks;

	}

	TEST_CASE("Allocate 1 byte and deallocate it. OK")
	{
		struct Dog_1
		{
			char a;
		};

		auto& mem_manager = MemoryManager::Get();
		FEngineMalloc* engineMalloc = new FEngineMalloc(*mem_manager.GetMemory());

		MemoryBlock** blocks = new MemoryBlock* [5];
		size_t allocatedNum = 0;
		Dog_1* obj = new (engineMalloc->Malloc_Debug(sizeof(Dog_1), *blocks, allocatedNum))(Dog_1);

		delete engineMalloc;
		delete blocks;
	}
		*/

	TEST_CASE("Allocate something larger than the cache line")
	{
		
	}

	TEST_CASE("Allocates, out of memory, exception.")
	{
		
	}
	TEST_CASE("Allocated memory, size bigger than Chunk's, OK ")
	{
		// We should have not the first memory block occupied only, but the first 2;
	}

	TEST_CASE("Get memory from the OS and then allocate the Global Allocator there, memory left counts with size minus the allocator's")
	{
		
	}


	TEST_CASE("Buffered Allocator")
	{


	}



#pragma endregion TEST

	//////////////////////////////




}// ishak::Memory
