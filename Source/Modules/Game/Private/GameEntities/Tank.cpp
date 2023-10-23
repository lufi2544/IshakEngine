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
		const float speed{ 10.0f };

		auto velocity{ (Vector2{ 10, 0 }) * (speed * deltaTime)};
		// Moving x amount every frame.
		m_position += velocity;		
	}

	void Tank::Fire()
	{
		
	}

}// ishak