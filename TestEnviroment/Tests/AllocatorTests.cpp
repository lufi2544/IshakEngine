#include "doctest.h"
#include "CoreMinimal.h"
#include "Memory/MemoryManager.h"

// STACK ALLOCATOR

namespace ishak::Memory {


	TEST_CASE("Allocated the whole engine memory, OK")
	{
		MemoryManager& memManger = MemoryManager::Get();
		memManger.AllocateInitialMemory();
		EngineMemory const* memory = memManger.GetMemory();

		CHECK(memory->PermanentMemory != NULL);
		CHECK(memory->TransientMemory != NULL);

		memManger.FreeEngineMemory();
	}


	TEST_CASE("Allocated Memory, the pointer is moved, OK")
	{

		const int N = 100;
		std::vector<std::pmr::list<int>*> system(N);

		// Allocator for the Lists ptrs resources (Nodes)
		NewDeleteAllocator allocator;
		for (int i = 0; i < N; ++i)
		{
			system[i] = new std::pmr::list<int>(&allocator);
		}


		for(auto element : system)
		{
			delete element;
		}

	}


	struct Dog
	{
		Dog() = default;
		Dog(int aa, int aaa)
		{
			a = aa;
			b = aaa;
		}
		~Dog()
		{
			a = 11;
		}
		int a, b;
	};


	TEST_CASE("Buffered Allocator")
	{


	}
}// ishak::Memory
