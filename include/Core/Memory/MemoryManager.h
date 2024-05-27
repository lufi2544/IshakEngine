#pragma once

#include "Core/CoreMinimal.h"
#include "Core/CoreConfig.h"
#include "Memory.h"
#include "Allocator.h"

namespace ishak{ namespace Memory{


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
		static MemoryManager& Get();

		MemoryManager() = default;
		MemoryManager(size_t memoryToAlloc);
		~MemoryManager();
		void InitMemory(size_t memoryToAlloc = 0);


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


}} // ishak::Memory
