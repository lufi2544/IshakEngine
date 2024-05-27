#include "Core/Memory/MemoryManager.h"
#if WINDOWS
	#include "Windows.h"
	#include "memoryapi.h"
	#include "stdlib.h"
#endif// WINDOWS

namespace ishak { namespace Memory {

	MemoryManager& MemoryManager::Get()
	{
		static MemoryManager lazySingleton;
		return lazySingleton;
	}

	MemoryManager::MemoryManager(size_t memoryToAlloc)
	{
		InitMemory(memoryToAlloc);
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

	void MemoryManager::InitMemory(size_t memoryToAllocate)
	{
		if (memoryToAllocate == 0)
		{
			memoryToAllocate = MemoryPages(kDefaultPagesToRequest);
		}

		assert(memoryToAllocate > 1);
		const int totalAllocatedPages = memoryToAllocate;
		const int memoryPerHirarchy = totalAllocatedPages / (uint8)(EMemoryHirarchy::NUM);
		const int memoryPerSandBox = memoryPerHirarchy / (uint8)(EMemorySandBoxType::NUM);

		for (uint8 i = 0; i < (uint8)EMemoryHirarchy::NUM; ++i)
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
			for (uint8 j = 0; j < (uint8)(EMemorySandBoxType::NUM); ++j)
			{
				size_t sandBoxSize = memoryPerSandBox;

				// if we are creating the last sandBox
				if (j == (uint8)(EMemorySandBoxType::NUM)-1)
				{
					int temp = associatedMemory + memoryPerSandBox;
					if (int memoryModuleToEnd = totalAllocatedPages - temp; memoryModuleToEnd < memoryPerSandBox)
					{
						// If there is some space left when we create this SandBox and that space is less than an entire
						// Sandbox, then we add it to the sandBox
						sandBoxSize += memoryModuleToEnd;
					}
				}

				sandboxes[i][j] = SandBox{ currentBufferPtr + associatedMemory, sandBoxSize };

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

}}// ishak::Memory
