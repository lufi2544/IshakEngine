#include "GameModule.h"
#include "GameInstance.h"
#include "TanksGameInstance.h"

namespace ishak {

	GameModule::GameModule()
	{

	}
	void GameModule::DoInitModule(Factory& f)
	{
		f.RegisterBuilder<GameInstance>([](Factory const&)
		{
			return std::make_shared<TanksGameInstance>();
		});
	}

}// ishak