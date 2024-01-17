#pragma once

#include "Core/CoreMinimal.h"
#include "Core/CoreConfig.h"
#include "Memory.h"

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

			~MemoryManager();
			size_t GetMemoryUsage() const { return m_allocatedMemory; }
			EngineMemory const* GetMemory() const { return &m_engineMemory; }

			/** Getter class. */
			static MemoryManager& Get();

			void* Allocate(size_t size);
			void Free(void* ptr, size_t size);
			void AllocateInitialMemory();
			void FreeEngineMemory();


	
	private:
			size_t m_allocatedMemory{ 0 };
			EngineMemory m_engineMemory;
	};

}} // ishak::Memory
