#include "doctest.h"
#include "Core/Memory/SharedPtr.h"

namespace ishak { namespace Memory {

	struct Animal
	{
		int id;
		// TODO Test with ishak::String
	};
#pragma region SHARED_PTR_TESTS
		//////////////// SHARED PTR ////////////////
		TEST_CASE("Creating Shared Ptr from an object, OK")
		{
		}

		TEST_CASE("MAKE_SHARED with no parameters, OK")
		{
			
		}

		TEST_CASE("MAKE_SHARED with parameters, OK")
		{
			
		}

		TEST_CASE("Creating Shared Ptr from an object, Memory Block, OK")
		{

		}

		TEST_CASE("Creating Shared Ptr and then add an object, OK")
		{

		}

		TEST_CASE("Copy constructor")
		{

		}

		TEST_CASE("Operator = ")
		{

		}

		TEST_CASE("Move Operator")
		{

		}

		TEST_CASE("Move Constructor")
		{

		}


		TEST_CASE("When strong count reaches 0, free main source")
		{

		}

		TEST_CASE("If strong counts do not reach 0, do not free source")
		{

		}

		TEST_CASE("operator equal to another shared, handling resource, stron refs > 0, assigned new.")
		{

		}

		TEST_CASE("operator equal to another shared, handling resource, strong refs == 0, free resource, assing new")
		{

		}
#pragma endregion SHARED_PTR_TESTS

#pragma region WEAK_PTR_TESTS



#pragma endregion WEAK_PTR_TESTS
	}}// ishak::Memory