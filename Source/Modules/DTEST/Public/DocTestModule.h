
#pragma once

#include "Module/Module.h"
#include "doctestConfig.h"

namespace ishak
{
	class DOCTEST_API DocTestModule : public Module
	{
	public:
		DocTestModule();
		void DoInitModule(Factory& f);
	};

	REGISTER_MODULE(DocTestModule);
}// ishak
