#include "Core/Memory/MemoryManager.h"
#if WINDOWS
#include "WIndows.h"
	#include "memoryapi.h"
#endif

namespace ishak { namespace Memory{

	MemoryManager& MemoryManager::Get()
	{
		static MemoryManager lazySingleton;
		return lazySingleton;
	}

	MemoryManager::~MemoryManager()
	{
		FreeEngineMemory();
	}

	void MemoryManager::FreeEngineMemory()
	{
		VirtualFree(m_engineMemory.PermanentMemory, m_engineMemory.PermanentMemorySize, MEM_RELEASE);
		VirtualFree(m_engineMemory.TransientMemory, m_engineMemory.TransientMemorySize, MEM_RELEASE);
	}

	void
	MemoryManager::AllocateInitialMemory()
	{
#if WINDOWS

		m_engineMemory.PermanentMemorySize = Megabytes(64);	
		m_engineMemory.PermanentMemory = VirtualAlloc(
			0,
			m_engineMemory.PermanentMemorySize,
			MEM_RESERVE|MEM_COMMIT,
			PAGE_READWRITE);

		m_engineMemory.TransientMemorySize = Gigabytes(4);
		m_engineMemory.TransientMemory = VirtualAlloc(
			0,
			m_engineMemory.TransientMemorySize,
			MEM_RESERVE|MEM_COMMIT,
			PAGE_READWRITE);

#elif LINUX

#endif // PLATFORMS
	}

	void*
	MemoryManager::Allocate(
			size_t size)
	{
		AllocationInfo* metadata = static_cast<AllocationInfo*>(malloc(size + sizeof(AllocationInfo)));
		if(metadata == nullptr || size > SIZE_MAX - sizeof(AllocationInfo))
		{
			// TODO MEMORY Exception

			int a = 0;
		}
		m_allocatedMemory += size;
		metadata->size = size;

		if (metadata->size > 100000)
		{
			int a = 0;
		}

		// this could be static_cast<void*>(metadata + 1)
		//void* ptr = static_cast<void*>(metadata);
		//return static_cast<void*>(ptr + sizeof(AllocationInfo));
		return static_cast<void*>(metadata + 1);
	}

	void
	MemoryManager::Free(
		void* ptr,
		size_t size)
	{
		// Retreiving the size of the allocation, as we allocated the memory block 1 byte further, to allocate the metadata
		// then we have to check in the memory address with 1 byte less.
		// This could be static_cast<AllocationInfo*>(ptr) - 1. Which 1 means ptrMemAdrres - (sizeof(AllocationInfo) * 1), but 
		// I like more the approach that we have right now, which is more readable;
		
		// This approach here for me is more readable but in windows it has compile errors due to the fact 
		// that the compiler (MSVS) does not like using the ptr that has no type for operations.
		//AllocationInfo* metadata = static_cast<AllocationInfo*>(ptr - sizeof(AllocationInfo)); 
		AllocationInfo* metadata = static_cast<AllocationInfo*>(ptr) - 1;
		m_allocatedMemory -= metadata->size;

		if(metadata->size > 100000)
		{
			int a = 0;
		}

		// freeing the metadata, as it is the memory address for the metadata and the ptr to free.
		free(metadata);
	}

}}// ishak::Memory
