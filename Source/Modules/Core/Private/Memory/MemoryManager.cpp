#include "Memory/MemoryManager.h"


namespace ishak { namespace Memory{

	MemoryManager& MemoryManager::Get()
	{
		static MemoryManager lazySingleton;
		return lazySingleton;
	}


	void* MemoryManager::Allocate(size_t size)
	{
		AllocationInfo* metadata = static_cast<AllocationInfo*>(malloc(size + sizeof(AllocationInfo)));
		if(metadata == nullptr)
		{
			// TODO MEMORY Exception
		}
		m_allocatedMemory += size;
		metadata->size = size;

		// this could be static_cast<void*>(metadata + 1)
		void* ptr = static_cast<void*>(metadata);
		return static_cast<void*>(ptr + sizeof(AllocationInfo));
	}

	void MemoryManager::Free(void* ptr, size_t size)
	{
		// Retreiving the size of the allocation, as we allocated the memory block 1 byte further, to allocate the metadata
		// then we have to check in the memory address with 1 byte less.
		// This could be static_cast<AllocationInfo*>(ptr) - 1. Which 1 means ptrMemAdrres - (sizeof(AllocationInfo) * 1), but 
		// I like more the approach that we have right now, which is more readable;
		AllocationInfo* metadata = static_cast<AllocationInfo*>(ptr - sizeof(AllocationInfo));
		m_allocatedMemory -= metadata->size;

		// freeing the metadata, as it is the memory address for the metadata and the ptr to free.
		free(metadata);
	}

}}// ishak::Memory
