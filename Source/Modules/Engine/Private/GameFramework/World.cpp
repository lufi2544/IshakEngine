#include "GameFramework/World.h"
#include "GameFramework/GameInstance.h"

namespace ishak {

	World::World(WeakPtr<GameInstance> GameInstanceParam)
		: m_GameInstance(GameInstanceParam)
	{

	}

	WeakPtr<GameInstance> World::GetGameInstance()
	{
		return m_GameInstance;
	}

	void World::Init()
	{

	}
	
	void World::Update(float deltaTime)
	{
		for(auto&& entity : m_WorldEntities)
		{
			entity->Tick(deltaTime);
		}
	}

}// ishak
