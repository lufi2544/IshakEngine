
#pragma once

#include "Core/Module/Module.h"
#include "EngineConfig.h"

namespace ishak
{
	class ENGINE_API EngineModule : public Module
	{
	public:
		EngineModule();
		void DoInitModule(Factory& f);
	};

	REGISTER_MODULE(EngineModule);
}// ishak