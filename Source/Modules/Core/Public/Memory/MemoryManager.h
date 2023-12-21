#pragma once

#include "CoreMinimal.h"
#include "CoreConfig.h"

namespace ishak{ namespace Memory{


struct AllocationInfo
{
	size_t size{ 0 };
};

	/** 
	 * Class that is in charge of managing the memory for the Engine. This includes allocation an deallocation of memory.
	 * */
	class CORE_API MemoryManager
	{
		public:
			MemoryManager() = default;
			MemoryManager(MemoryManager const& other) = delete;
			MemoryManager(MemoryManager&& other) = delete;
			MemoryManager& operator = (MemoryManager const& other) = delete;
			MemoryManager& operator = (MemoryManager&& other) = delete;
	
			/** Getter class. */
			static MemoryManager& Get();

			void* Allocate(size_t size);
			void Free(void* ptr, size_t size);
			size_t GetMemoryUsage() const { return m_allocatedMemory; }
	
			size_t m_allocatedMemory{ 0 };
	};

}} // ishak::Memory
