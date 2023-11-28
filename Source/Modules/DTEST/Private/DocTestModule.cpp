#include "DocTestModule.h"
#include "doctest.h"
#include "CoreMinimal.h"

#ifndef LINUX // TODO change this for if with Windows
	#include <Windows.h>
#endif

namespace ishak{

	DocTestModule::DocTestModule()
	{

	}

	void DocTestModule::DoInitModule(Factory& f)
	{
		doctest::Context(0, nullptr).run();
	}
}// ishak

