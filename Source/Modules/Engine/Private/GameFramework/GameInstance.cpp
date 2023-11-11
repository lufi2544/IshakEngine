// ISHAK
#include "GameFramework/GameInstance.h"

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

	void GameInstance::SetEcsContext(TArray<SharedPtr<Ecs::EcsContext>>* context)
	{
		m_ecsContextPtr = context;
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
