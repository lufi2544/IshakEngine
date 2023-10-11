#include "GameModule.h"
#include "World/GameInstance.h"
#include "TanksGameInstance.h"

namespace ishak {

	GameModule::GameModule()
	{

	}
	void GameModule::DoInitModule(Factory& factory)
	{
		factory.RegisterBuilder<GameInstance>([](Factory const&)
		{
			return std::make_shared<TanksGameInstance>();
		});
	}

}// ishak