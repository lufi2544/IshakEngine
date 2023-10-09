// ISHAK
#include "GameInstance.h"
#include "World.h"


namespace ishak{

	void GameInstance::Init()
	{
		// Init general stuff: world, main managers...
		m_world = std::make_shared<World>();

		DoInit();
	}

	std::weak_ptr<World> GameInstance::GetWorld()
	{
		return m_world;
	}

	std::weak_ptr<World> GameInstance::GetWorld() const
	{
		return m_world;
	}

}//ishak
