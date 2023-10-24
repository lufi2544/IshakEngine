
#pragma once

#include "Module/Module.h"
#include "docTestConfig.h"

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