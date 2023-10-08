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

}//ishak
