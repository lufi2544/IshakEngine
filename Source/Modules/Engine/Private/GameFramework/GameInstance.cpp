// ISHAK
#include "GameFramework/GameInstance.h"
#include "Log/Logger.h"

namespace ishak{

	void GameInstance::Init()
	{
		// Init general stuff: world, main managers...
		m_world->Init();
		
		DoInit();
	}

	void GameInstance::SetWorld(World* world)
	{		
		m_world = world;
	}

	void GameInstance::SetEcsContext(Ecs::EcsContextContainer* engineContextContainer)
	{
		ISHAK_LOG(Temp, "Initializing GAME ECS..................")
		Ecs::EcsContext* customGameEcsContext{ CreteCustomEcsContext(engineContextContainer->entityManger) };
		engineContextContainer->AddContext(customGameEcsContext);
		m_ptrEcsContextContainer = engineContextContainer;
	}

	Ecs::EcsContext* GameInstance::CreteCustomEcsContext(SharedPtr<Ecs::EntityManager> entityManager)
	{
		UniquePtr<Ecs::ComponentManipulator> componentManipulator{ std::make_unique<Ecs::ComponentManipulator>() };
		Ecs::EcsContext* gameEcsContext{ new Ecs::EcsContext(entityManager, std::move(componentManipulator)) };

		RegisterEcsGameContainers(gameEcsContext->GetComponentManipulator());
		RegisterEcsGameSystems(gameEcsContext);

		return gameEcsContext;
	}

	void GameInstance::RegisterEcsGameContainers(Ecs::ComponentManipulator* compMan)
	{
		//...
		DoRegisterEcsGameContainers(compMan);
	}

	void GameInstance::RegisterEcsGameSystems(Ecs::EcsContext* ecsContext)
	{
		//...

		DoRegisterEcsGameSystems(ecsContext);
	}

	void GameInstance::ShutDown()
	{
		m_world->ShutDown();
	}

	World* GameInstance::GetWorld()
	{
		return m_world;
	}

	World* GameInstance::GetWorld() const
	{
		return m_world;
	}


}//ishak
