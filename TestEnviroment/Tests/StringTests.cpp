#include "doctest.h"
#include "Core/CoreMinimal.h"

namespace ishak {


	TEST_CASE("Copy construct from char* DEEP, OK")
	{
		char buff[6] = "Hello";
		buff[5] = String::nullChar;

		ishak::String str{ "Hello" };
		TArray<char>* buffer = str.GetBuffer();

		bool bAllTrue = false;
		
		for(int idx = 0; idx < 6; ++idx)
		{
			bAllTrue = (buff[idx] == (*(buffer))[idx]);
			if(!bAllTrue)
			{
				break;
			}
		}
		

		CHECK(buffer->Size() == 6 /* Hello + nullchar */);
		CHECK(bAllTrue == true);
	}

	TEST_CASE("operator = from String, DEEP, OK")
	{
		String str1{ "Yes"};
		String str2{ "No" };

		str2 = str1;

		auto buffer1 = str1.GetBuffer();
		auto buffer2 = str2.GetBuffer();

		
		CHECK(*buffer1 == *buffer2);
	}

	TEST_CASE("Find a word in a string, true")
	{
		ishak::String string{ "aakkaaakfaksljdlkdfjabhaklsdjfaskjdf0askjdf0alskdjf0askljf" };
		bool bfound{ string.Find("abh") };

		CHECK(bfound == true);
	}

	TEST_CASE("Find a word in a string, false")
	{
		ishak::String string{ "aakkaaakfaksljdlkdfjabhaklsdjfaskjdf0askjdf0alskdjf0askljf" };
		bool bfound{ string.Find("zzz") };

		CHECK(bfound == false);
	}

	TEST_CASE("Find a word in an empty string, false")
	{
		ishak::String string;
		bool bfound{ string.Find("zzz") };

		CHECK(bfound == false);
	}

	TEST_CASE("Move string memory.")
	{
		ishak::String source{ "hello" };

		ishak::String destination;
		destination = std::move(source);

		CHECK(source.IsEmpty() == true);
		CHECK((destination == ishak::String("hello")) == true);
	}

	TEST_CASE("Empty string memory.")
	{
		ishak::String str;

		CHECK(str.IsEmpty());
	}

	TEST_CASE("Adding up 2 strings, true")
	{
		ishak::String word1{ "hello" };
		ishak::String word2{ " world!!" };

		ishak::String final = word1 + word2;

		CHECK(final == ishak::String("hello world!!"));
	}

	TEST_CASE("Adding up 2 strings, false")
	{
		ishak::String word1{ "hello" };
		ishak::String word2{ " world!!" };

		ishak::String final = word1 + word2;

		CHECK((final == ishak::String("hello world")) == false);
	}

	TEST_CASE("Adding up a string and a char*, true")
	{
		ishak::String word1{ "hello" };
		ishak::String final = word1 + " world!!";

		CHECK(word1.Size() == 5);
		CHECK(final.Size() == (word1.Size() + 8));
		CHECK((final == ishak::String("hello world!!")) == true);
	}

	TEST_CASE("Adding up a string and a char*, false")
	{
		ishak::String word1{ "hello" };
		ishak::String final = word1 + " world!!";
		 
		// TODO Check size

		CHECK(word1.Size() == 5);
		CHECK(final.Size() == (word1.Size() + 8));
		CHECK((final == ishak::String("hello world")) == false);
	}

	TEST_CASE("Merging two strings, true")
	{
		ishak::String word1{ "yes" };
		const size_t sizeBeforeAddition{ word1.Size() };
		ishak::String final{ " no!" };


		word1 += final;

		CHECK(word1.Size() == (sizeBeforeAddition + final.Size()));
		CHECK((word1 == ishak::String("yes no!")) == true);
	}
}