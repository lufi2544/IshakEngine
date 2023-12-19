#include "doctest.h"
#include "DataContainers/HashMap.h"

#include <map>

namespace ishak{namespace Tests{

	typedef THashMap<String, int> THMap;

	template<class K, class V>
	void PrintHashMapStats(THashMap<K, V> const& map )
	{
		std::cout << "Num of buckets: "<< map.GetBucketsNum() << " Size: " << map.Size() << std::endl;
	}


	template<class K, class V>
	void PrintBuckets(THashMap<K, V> map)
	{
		size_t idx = 0;
		for(TList<TPair<K, V>> const& bucketElement : map.GetBuckets())
		{
			std::cout << "Bucket: " << idx << " Size: " << bucketElement.Size() << std::endl; 
			for(TPair<K, V> const& bucketPair : bucketElement)
			{
				std::cout << bucketPair.key << std::endl; 	 		
			}

			++idx;
		}
	}
	

	TEST_CASE("Copy constructor, OK")
	{
		THMap map;
		map["Hello"] = 4;

		map["Helloo"] = 5;
		map["Hellooo"] = 6;

		THMap newMap{ std::move(map) };

		CHECK(newMap["Hello"] == 4);
		CHECK(newMap["Helloo"] == 5);
		CHECK(newMap["Hellooo"] == 6);

		CHECK(map["Hello"] == 0);
		CHECK(map["Helloo"] == 0);
	}

	TEST_CASE("Move Constructor, OK")
	{

		THMap map;
		map["Hello"] = 4;
		map["Helloo"] = 5;
		map["Hellooo"] = 6;

		THMap newMap{ std::move(map) };

		CHECK(newMap["Hello"] == 4);
		CHECK(newMap["Helloo"] == 5);
		CHECK(newMap["Hellooo"] == 6);

		CHECK(map.Contains("Hello") == false);
		CHECK(map.Contains("Helloo") == false);
		CHECK(map.Size() == 0);
	}

	TEST_CASE("Operator =, OK")
	{
		THMap map;
		map["Hello"] = 4;
		map["Helloo"] = 5;
		map["Hellooo"] = 6;

		THMap newMap;
		newMap = map;

		CHECK(newMap["Hello"] == 4);
		CHECK(newMap["Helloo"] == 5);
		CHECK(newMap["Hellooo"] == 6);

		CHECK(map["Hello"] == 4);
		CHECK(map["Helloo"] == 5);
		CHECK(map["Hellooo"] == 6);
	}
	TEST_CASE("Iterated through emtpty HashMap, OK")
	{
		THMap map;
		int iteratedTimes = 0;

		for(auto pair : map)
		{
			++iteratedTimes;
		}

		CHECK(iteratedTimes == 0);
	}


	TEST_CASE("Iterated through Map, changed value, OK")
	{
		THMap map;
		const String key = { "Hello" };
		int oldValue = { 1 };
		int newValue = { 11 };
		map.Add(MakePair<String, int>(key, oldValue));
		map.Add(MakePair<String, int>("ouuu", 22));
		map.Add(MakePair<String, int>("ouuuu", 22));
		map.Add(MakePair<String, int>("ouuuuu", 22));
		map.Add(MakePair<String, int>("ouuuuuu", 22));
		map.Add(MakePair<String, int>("ouuuuuuu", 22));

		size_t timesIterated{ 0 };

		for(TPair<String, int>& mapPair : map)
		{
			timesIterated++;
			if(mapPair.key == "Hello")
			{
				mapPair.value = newValue;

			}else if(mapPair.key == "ouuuuu")
			{
				mapPair.value = 20000;
			}
			
		}

		int& gotValue = { map[key] };

		CHECK(gotValue == newValue);
		CHECK(map["ouuuuu"] == 20000);
		CHECK(timesIterated == 6);
	}

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
		CHECK(hmap.Size() == 1);

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
		hmap.Add(MakePair<String, int>("Hello", newValue));
		hmap.Add(MakePair<String, int>("Hello", newValue));
		hmap.Add(MakePair<String, int>("Hello", newValue));
		hmap.Add(MakePair<String, int>("Hello", newValue));
		hmap.Add(MakePair<String, int>("Hello", newValue));
		hmap.Add(MakePair<String, int>("Hello", newValue));
		hmap.Add(MakePair<String, int>("Hello", newValue));
		hmap.Add(MakePair<String, int>("Hello", newValue));

		auto& gotValue{ hmap[key] };
		CHECK(gotValue == newValue);
		CHECK(hmap.Size() == 1);
	}


	TEST_CASE("Delete a key, then the size, Ok and not contained, OK ")
	{
		THMap map;
		map.Add(MakePair<String, int>("Hello", 2));
		map.Remove("Hello");

		CHECK(map.Contains("Hello") == false);
	}



	TEST_CASE("Added the Doubly linked list, OK!")
	{

	}

}}// ishak::Tests
