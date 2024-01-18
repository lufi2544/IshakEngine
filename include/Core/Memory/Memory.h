#pragma once

// In this file I am going to add the different definitions/classes related to the memory
// We can say that the allocation of memory along other stuff.

#include "Core/CoreMinimal.h"
#include "Core/CoreConfig.h"
// Global memory operator override


#define Kilobytes(v) ((uint64)(v) * 1024)
#define Megabytes(v) (Kilobytes((uint64)(v)) * 1024)
#define Gigabytes(v) (Megabytes((uint64)(v)) * 1024)


namespace ishak{ namespace Memory{

struct CORE_API EngineMemory
{
	uint64 PermanentMemorySize;
	void* PermanentMemory;

	uint64 TransientMemorySize;
	void* TransientMemory;
};

}}// ishak::Memory

#define CUSTOM_MEMORY_ALLOC

#ifdef CUSTOM_MEMORY_ALLOC

CORE_API extern class FMalloc* GAlloc;

// For now this is okay, but We will request some memory chunk beforehand for the Engine
// systems memory usage.
class CORE_API FUseSystemMallocForNew
{
public:
	virtual ~FUseSystemMallocForNew() = default;

	void* operator new(size_t size);

	void operator delete(void* ptr);

	void* operator new[](size_t size);

	void operator delete[](void* ptr);
};


class CORE_API FMalloc : public FUseSystemMallocForNew
{
public:
	virtual void* Malloc(size_t size) = 0;

	virtual void Free(void* ptr) = 0;
};



class CORE_API FEngineMalloc : public FMalloc
{
public:
	FEngineMalloc() = default;

	void* Malloc(size_t size) override;
	void Free(void* ptr) override;
};

#endif
