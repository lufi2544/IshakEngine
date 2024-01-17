#include "Core/Memory/Memory.h"
#include "Core/Memory/MemoryManager.h"

/*
void* operator new (ishak::size_t size)
{
	return ishak::Memory::MemoryManager::Get().Allocate(size); 
}

void* operator new[](ishak::size_t size)
{
	return ishak::Memory::MemoryManager::Get().Allocate(size);
}

void operator delete(void* ptr)
{
	if(ptr == nullptr)
	{
		return;
	}
	ishak::Memory::MemoryManager::Get().Free(ptr, 0);
}

void operator delete[](void* ptr)
{
	if (ptr == nullptr) 
	{
		return;
	}
	ishak::Memory::MemoryManager::Get().Free(ptr, 0);	
}
*/
