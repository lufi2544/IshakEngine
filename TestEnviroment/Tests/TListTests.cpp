#include <doctest.h>

#include "DataContainers/List.h"
#include "DataContainers/Array.h"

namespace ishak{ namespace Tests{


	/** Returns the first element of th elist that is equal to the passed value. */
	template<typename T>
	bool ListContains(TList<T> const& list, T const& val)
	{
		for(T& element : list)
		{
			if(element == val)
			{
				return true;
			}
		}

		return false;
	}

	TEST_CASE("Added 2 elements, first one deleted(head), the second one should be the head now")
	{
		TList<int> list;

		list.Add(0);
		list.Add(1);

		list.Remove(0);


		for(int& element : list)
		{
			CHECK(element == 1);
		}
		
	}


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
		TList<int> list;
		list.Add(1);
		list.Add(2);
		list.Add(3);
		list.Add(4);
		list.Add(5);

		int idxToRemove{ 3 };
		list.Remove(idxToRemove);
	
		bool bContains{ false };
		for(auto& element : list)
		{
			if(element == 4)
			{
				bContains = true;
				break;
			}	
		}

		CHECK(bContains == false);

	}

	TEST_CASE("2 lists empty are equal, OK")
	{
		TList<int> list;
		TList<int> list1;

		CHECK(list == list1);
	}

	TEST_CASE("Getting element idx, then removing it. OK")
	{
		TList<int> list;
		list.Add(0);
		list.Add(1);
		list.Add(2);
		list.Add(3);
		size_t foundIdx{ list.Find(3) };
		list.Remove(foundIdx);

		bool const bContains = ListContains(list, 3);
		CHECK(bContains == false); 
	}

	TEST_CASE("Getting an element, modifying it, then accessing it. OK")
	{

		TList<int> list;
		list.Add(0);
		list.Add(1);
		list.Add(2);
		list.Add(3);
		list.Add(4);
		list.Add(5);

		for(int& element : list)
		{
			if(element == 0)
			{
				element = 100;
			}
		}

		bool const bContainsNumber = { ListContains(list, 100) };
		bool const bNotContainingOldNumber = { !ListContains(list, 0) };
		CHECK(bContainsNumber == true);
		CHECK(bNotContainingOldNumber == true);

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
