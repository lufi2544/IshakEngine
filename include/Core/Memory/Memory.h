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

 void* operator new (size_t size);
 void operator delete (void* ptr);


void* operator new [] (size_t size);
void operator delete[](void* ptr);

#endif
