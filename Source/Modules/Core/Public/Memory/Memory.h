#pragma once

// In this file I am going to add the different definitions/classes related to the memory
// We can say that the allocation of memory along other stuff.

#include "CoreMinimal.h"
#include "CoreConfig.h"
// Global memory operator override


 void* operator new (size_t size);
 void operator delete (void* ptr);


void* operator new [] (size_t size);
void operator delete[](void* ptr);




