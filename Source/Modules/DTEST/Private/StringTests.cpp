#include "IshakTest.h"

TEST_CASE("Find a word in a string, true")
{
	ishak::String string{"aakkaaakfaksljdlkdfjabhaklsdjfaskjdf0askjdf0alskdjf0askljf"};
	bool bfound{ string.Find("abh") };

	CHECK(bfound == true);
}

TEST_CASE("Find a word in a string, false")
{
	ishak::String string{"aakkaaakfaksljdlkdfjabhaklsdjfaskjdf0askjdf0alskdjf0askljf"};
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
	ishak::String source{"hello"};
			
	ishak::String destination;
	destination = std::move(source);
	 		
	CHECK(source.IsEmpty() == true);	
	CHECK((destination == ishak::String("hello")) == true);
}

TEST_CASE("Adding up 2 strings, true")
{
	ishak::String word1{"hello"};
	ishak::String word2{" world!!" };
	
	ishak::String final = word1 + word2;

	CHECK(final == ishak::String("hello world!!"));
}

TEST_CASE("Adding up 2 strings, false")
{
	ishak::String word1{"hello"};
	ishak::String word2{" world!!" };

	ishak::String final = word1 + word2;

	CHECK((final == ishak::String("hello world")) == false);
}

TEST_CASE("Adding up a string and a char*, true")
{
	ishak::String word1{"hello"};	

	ishak::String final = word1 + " world!!";

	CHECK((final == ishak::String("hello world!!")) == true);
}

TEST_CASE("Adding up a string and a char*, false")
{
	ishak::String word1{"hello"};

	ishak::String final = word1 + " world!!";

	CHECK((final == ishak::String("hello world")) == false);
}

TEST_CASE("Merging two strings, true")
{
	ishak::String word1{"hello"};
	ishak::String final{ " world!!" };

	word1 += final;

	CHECK((word1 == ishak::String("hello world!!")) == true);
}