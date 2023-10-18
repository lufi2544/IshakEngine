
#pragma once

#include "Module/Module.h"
#include "LaunchConfig.h"

namespace ishak
{
	class LAUNCH_API LaunchModule : public Module
	{
	public:
		LaunchModule();
		void DoInitModule(Factory& f);
	};

	REGISTER_MODULE(LaunchModule);
}// ishak