#include "CoolTest.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

namespace ishak {

	CoolTest::CoolTest()
	{

	}
}

TEST_CASE("Just Chilling")
{
	ishak::CoolTest cooltest;

	CHECK(cooltest.Data == 4);
}