#include "GameModule.h"
#include "GameFramework/GameInstance.h"
#include "TanksGameInstance.h"

// ECS
#include "ComponentManipulator.h"
#include "ComponentContainer.h"

namespace ishak {

	GameModule::GameModule()
	{

	}
	void GameModule::DoInitModule(Factory& factory)
	{
		factory.RegisterBuilder<GameInstance>([](Factory const&)
		{
			return std::make_shared<TanksGameInstance>();

		}, EClassMultiplicity::Singleton);



		Ecs::ComponentManipulator manipulator;


		
	}


	
	
	
	

}// ishak