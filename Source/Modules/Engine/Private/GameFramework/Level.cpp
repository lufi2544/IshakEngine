#include "GameFramework/Level.h"

#include "GameFramework/World.h"
#include "Ecs/Components/LevelComponent.h"
#include "GameFramework/GameInstance.h"


namespace ishak
{
	Level::Level(World* world)
		: m_world(world)
	{
	}

	void Level::Init()
	{
		
		/*
		Ecs::EcsContext* ecsContext{ m_world->GetGameInstance().lock()->GetEcsContext() };
		entityId = ecsContext->GetEntityManager()->RegisterEntity(this);

		LevelComponent levelComponent{ {0, 0, 0, 0} };
		ecsContext->GetComponentManipulator()->AddComponentDeferred<LevelComponent>(entityId, levelComponent);
		*/
	}
}//ishak