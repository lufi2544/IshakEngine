#include "doctest.h"
#include "CoreMinimal.h"

namespace ishak {
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
		for (int idx = 0; idx < array.Size(); ++idx)
		{
			if (idx == array.Size() - 1)
			{
				lastElement = array[idx];
			}
		}

		CHECK(array.Size() == 6);
		CHECK(array.Capacity() == 6);
		CHECK(lastElement == 5);
	}

	TEST_CASE("Array equals another array, OK")
	{
		ishak::TArray<int> array0{0, 1};
		ishak::TArray<int> array1;

		array1 = array0;

		CHECK((array0 == array1) == true);
	}

	TEST_CASE("Add x defaulted elements to empty array , OK")
	{
		struct Element
		{
			Element() = default;

			int data{ 4 };
			ishak::String name;
		};

		ishak::TArray<Element> container;
		container.AddDefaulted(5);

		CHECK(container.Size() == 5);
	}

	TEST_CASE("Add x defaulted elements to NON-empty array , OK")
	{
		struct Element
		{
			Element() = default;

			int data{ 4 };
			ishak::String name;
		};

		Element e1{ 4, "Entity" };
		Element e2{ 5, "Doc" };

		TArray<Element> container;
		container.Add(e1);
		container.Add(e2);
		uint32 oldSize{ container.Size() };

		uint32 dataToAdd{ 5 };
		container.AddDefaulted(dataToAdd);

		CHECK(container.Size() == oldSize + dataToAdd);
	}

}// ishak
