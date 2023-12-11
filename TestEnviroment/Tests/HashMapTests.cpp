#include "doctest.h"
#include "DataContainers/HashMap.h"

#include <map>

namespace ishak{namespace Tests{

	typedef THashMap<String, int> THMap;

	TEST_CASE("Created Map, OK")
	{
		THMap hmap;
		const size_t bucketsNum = { hmap.GetBucketsNum() };

		CHECK(bucketsNum == THMap::INITIAL_BUCKETS);
	}



	TEST_CASE("Added pair to the map, OK")
	{
		THMap hmap;
		hmap.Add(MakePair<String, int>("Hello", 2));

		const size_t mapsize = { hmap.Size() };
		CHECK(mapsize == 1);
	}


	TEST_CASE("Added pair to the map, get the value added. OK")
	{
		THMap hmap;
		String keyToAdd{ "Hello" };
		int valueToAdd{ 2 };

		hmap.Add(MakePair<String, int>(keyToAdd, valueToAdd));

		const int value { hmap[keyToAdd] };
		CHECK(value == valueToAdd);

	}

	TEST_CASE("Added pair to the map, is not added, then default value will be returned, OK")
	{
		THashMap<int, String> hmap;
	    int keyToAdd{ 11 };

		const String value { hmap[keyToAdd] };
		CHECK(value == "");

	}

	// Having the load factor below 0.8, which means allocating more buckets at the begining, hashmaps are intended for constant look up time for 
	// value-key pairs. 
	TEST_CASE("Added a pair, the value is already in the hash map, override, OK")
	{
		THMap hmap;
		String const key = { "Hello" };
		int oldValue = { 22 };

		hmap.Add(MakePair<String, int>(key, oldValue));

		int newValue = { 220 };
		hmap.Add(MakePair<String, int>("Hello", newValue));

		auto& gotValue{ hmap[key] };
		CHECK(gotValue == newValue);
	}


	TEST_CASE("Added pair, the growth ratio is greater than expected, we rehash and add after OK, Size, OK, Buckets. OK")
	{

	}


	TEST_CASE("Delete a key, then the size, Ok and not contained, OK ")
	{

	}

	TEST_CASE("Added the Doubly linked list, OK!")
	{

	}

}}// ishak::Tests
