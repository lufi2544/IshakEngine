#include "GameFramework/World.h"
#include "GameFramework/GameInstance.h"
#include "GameFramework/Level.h"

namespace ishak {

	World::World(WeakPtr<GameInstance> GameInstanceParam)
		: m_GameInstance(GameInstanceParam)
	{

	}

	void World::Init()
	{
		m_level = std::make_shared<Level>(this);
		m_level->Init();
	}
	
	void World::Update(float deltaTime)
	{
		for(auto&& entity : m_WorldEntities)
		{
			entity->Tick(deltaTime);
		}
	}

	void World::ShutDown()
	{
		
		auto gameInstance{ GetGameInstance().lock() };
		if (!gameInstance)
		{
			return;
		}

		// TODO Refactor ECS remove the entities from all the ecsContexts

		Ecs::EcsContext* ecsRendererContext{ GetGameInstance().lock()->GetEcsContext(Ecs::ContextID::RENDERER) };
		
		if (!ecsRendererContext)
		{
			return;
		}

		for (auto&& entity : m_WorldEntities)
		{
			ecsRendererContext->UnregisterEntity(entity.get());
		}
	}

}// ishak
