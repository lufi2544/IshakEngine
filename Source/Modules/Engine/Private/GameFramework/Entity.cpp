#include "GameFramework/Entity.h"

namespace ishak {

	Entity::Entity(World* world)
		: m_World(world)
	{

	}
	void Entity::BeginPlay()
	{
		DoOnBeginPlay();
	}

	void Entity::Update(float deltaTime)
	{		
		DoUpdate(deltaTime);
	}
}// ishak