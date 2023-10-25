#include "DocTestModule.h"
#include "doctest.h"
#include "CoreMinimal.h"
#include <Windows.h>

namespace ishak{

	DocTestModule::DocTestModule()
	{

	}

	void DocTestModule::DoInitModule(Factory& f)
	{
		doctest::Context(0, nullptr).run();
	}
}// ishak

