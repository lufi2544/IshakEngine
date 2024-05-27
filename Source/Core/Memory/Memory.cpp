#include "Core/Memory/Memory.h"

#include "Core/CoreConfig.h"
#include "Core/Memory/MemoryManager.h"
#include "Core/Memory/MemoryTypes.h"

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
