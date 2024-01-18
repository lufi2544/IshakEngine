#include "Core/Memory/Memory.h"
#include "Core/Memory/MemoryManager.h"

#ifdef CUSTOM_MEMORY_ALLOC

CORE_API FMalloc* GAlloc = nullptr;

#endif


void* FUseSystemMallocForNew::operator new (size_t size)
{
	return FMemory::SystemMalloc(size);
}

void FUseSystemMallocForNew::operator delete(void* ptr)
{
	FMemory::SystemFree(ptr);
}

void* FUseSystemMallocForNew::operator new[](size_t size)
{
	return FMemory::SystemMalloc(size);
}

void FUseSystemMallocForNew::operator delete [](void* ptr)
{
	FMemory::SystemFree(ptr);
}

void* FEngineMalloc::Malloc(size_t size)
{
	return ishak::Memory::MemoryManager::Get().Allocate(size);
}

void FEngineMalloc::Free(void* ptr)
{
	ishak::Memory::MemoryManager::Get().Free(ptr, 0);
}
