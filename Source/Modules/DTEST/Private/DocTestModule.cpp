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
		char* ff = new char[1000];

		doctest::Context(0, &ff).run();

		std::string g{ ff };
		int a = 0;
		

		delete ff;

		ff = nullptr;
	}
}// ishak

