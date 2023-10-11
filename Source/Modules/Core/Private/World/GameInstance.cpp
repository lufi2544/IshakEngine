// ISHAK
#include "World/GameInstance.h"
#include "World/World.h"


namespace ishak{

	void GameInstance::Init()
	{
		// Init general stuff: world, main managers...
		m_world = std::make_shared<World>();		

		DoInit();
	}

	WeakPtr<World> GameInstance::GetWorld()
	{
		return m_world;
	}

	WeakPtr<World> GameInstance::GetWorld() const
	{
		return m_world;
	}

}//ishak
