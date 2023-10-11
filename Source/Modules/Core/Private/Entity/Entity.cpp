#include "Entity/Entity.h"

namespace ishak {

	Entity::Entity(WeakPtr<World> world)
		: m_World()
	{

	}
	void Entity::BeginPlay()
	{
		DoOnBeginPlay();


	}
}// ishak