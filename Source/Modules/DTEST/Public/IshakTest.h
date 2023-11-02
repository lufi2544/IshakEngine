#pragma once

// Just for now, simulates the compiler preprocessor implementation.
#define WITH_TESTS

#ifndef WITH_TESTS
	#define RUN_TESTS 0
#else 
	#define RUN_TESTS 1
#endif // !WITH_TESTS

#include "doctest.h"
#include "CoreMinimal.h"

#if RUN_TESTS
	#define IMPLEMENT_TESTS DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#endif // RUN_TESTS
