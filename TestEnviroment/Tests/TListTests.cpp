#include <doctest.h>

#include "DataContainers/TList.h"

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

	}





}}// ishak::Tests
