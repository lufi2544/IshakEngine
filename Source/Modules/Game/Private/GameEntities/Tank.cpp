#include "GameEntities/Tank.h"
#include "FileSystem/FileSystem.h"
#include "Log/Logger.h"

namespace ishak{

	Tank::Tank(World* world)
		: Entity(world)
	{

	}

	void Tank::DoOnBeginPlay()
	{
		String tankTextureName{ "Tank_Right.png"};

		m_renderContext.texturePath = (FileSystem::Get().GetAssetsDir() + tankTextureName);

		ISHAK_LOG("Tank Created", Temp)
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