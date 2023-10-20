#include "GameEntities/Tank.h"

namespace ishak{

	Tank::Tank(World* world)
		: Entity(world)
	{

	}

	void Tank::DoOnBeginPlay()
	{
		m_renderContext.texture = "../../Content/Tank_Right.png";
	}

	void Tank::DoTick(float deltaTime)
	{
		
	}

	void Tank::Fire()
	{
		
	}

}// ishak