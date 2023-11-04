#include "IshakTest.h"

TEST_CASE("Array Resizes, need reallocation for more elements, new size is OK and Capacity is OK")
{
	ishak::TArray<int> array{ 1, 1, 1 };

	array.Resize(10);

	CHECK(array.Size() == 10);	
	CHECK(array.Capacity() == 10);
}

TEST_CASE("Array Reserves, new size is OK and Capacity is OK")
{
	ishak::TArray<int> array{ 1, 1, 1 };
	auto initialSize{ array.Size() };

	array.Reserve(10);

	CHECK(array.Size() == initialSize);
	CHECK(array.Capacity() == 10);
}

TEST_CASE("Array Resizes, new allocation needed for less elemets, OK")
{
	ishak::TArray<int> array{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 }; // 20 elements
	auto actualSize{ array.Size() };
	auto deltaSize = 5;	
	array.Resize(6); // Last element should be 5

	int lastElement{ 0 };
	for(int idx = 0; idx < array.Size(); ++idx)
	{
		if(idx == array.Size() - 1)
		{
			lastElement = array[idx];
		}
	}

	CHECK(array.Size() == 6);
	CHECK(array.Capacity() == 6);
	CHECK(lastElement == 5);
}