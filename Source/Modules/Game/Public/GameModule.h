
#pragma once

#include "Module/Module.h"
#include "GameConfig.h"

namespace ishak
{
	class GAME_API GameModule : public Module
	{
	public:
		GameModule();
		void DoInitModule(Factory& f);
	};

	REGISTER_MODULE(GameModule);
}// ishak