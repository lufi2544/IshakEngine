#include <doctest.h>

#include "DataContainers/List.h"
#include "DataContainers/Array.h"

namespace ishak{ namespace Tests{

	TEST_CASE("Add an element, OK")
	{
		TList<int> list;

		list.Add(4);
		list.Add(5);
		list.Add(6);
		list.Add(7);
		list.Add(8);

		bool bFound{ false };
		int toFind{ 7 };
		for(const int& n : list)
		{
			if(toFind == n)
			{
				bFound = true;
				break;
			}
		}


		CHECK(bFound == true);		
	}

	TEST_CASE("Add element, size OK")
	{
		TList<int> list;
		TArray<int> elemetsToAdd  = { 11, 44, 4, 1234, 1243, 11234, 11234, 132412,  1243, 11234, 11234, 132412,
			1243, 11234, 11234, 132412,  1243, 11234, 11234, 132412,  1243, 11234, 11234, 132412,  };

		for(auto& toAdd : elemetsToAdd)
		{
			list.Add(toAdd);			
		}

		CHECK(list.Size() == elemetsToAdd.Size());
	}	


	TEST_CASE("Remove Element, Size, Not contained, OK")
	{

	}

	TEST_CASE("Getting an element, modifying it, then accessing it. OK")
	{

	}

	TEST_CASE("Copying a list, OK")
	{
		TList<int> list1;

		list1.Add(1);
		list1.Add(2);
		list1.Add(3);
		list1.Add(4);
		list1.Add(5);
		list1.Add(6);

		TList<int> list2 = { list1 };

		CHECK(list2 == list1);	
	}

}}// ishak::Tests
