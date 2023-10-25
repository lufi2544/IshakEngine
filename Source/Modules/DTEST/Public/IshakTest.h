#pragma once

// Just for now, simulates the compiler preprocessor implementation.
#define WITH_TESTS 1

#ifndef WITH_TESTS
	#define WITH_TESTS 0
#endif // !WITH_TESTS

#include "doctest.h"
#include "CoreMinimal.h"

#if WITH_TESTS
	#define IMPLEMENT_TESTS DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#endif // WITH_TESTS
