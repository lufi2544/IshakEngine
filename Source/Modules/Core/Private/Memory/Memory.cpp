#include "Memory/Memory.h"
#include "Memory/MemoryManager.h"

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
	ishak::Memory::MemoryManager::Get().Free(ptr, 0);
}

void operator delete[](void* ptr)
{

	ishak::Memory::MemoryManager::Get().Free(ptr, 0);	
}
