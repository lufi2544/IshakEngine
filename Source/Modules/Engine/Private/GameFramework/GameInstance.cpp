// ISHAK
#include "GameFramework/GameInstance.h"

namespace ishak{

	void GameInstance::Init()
	{
		// Init general stuff: world, main managers...
		
		DoInit();
	}

	void GameInstance::SetWorld(World* world)
	{
		m_world = world;
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
