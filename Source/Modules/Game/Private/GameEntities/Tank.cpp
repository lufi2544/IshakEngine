#include "GameEntities/Tank.h"
#include "FileSystem/FileSystem.h"
#include "Log/Logger.h"

namespace ishak{

	Tank::Tank(World* world)
		: Entity(world)
	{

	}

	Tank::Tank(World* world, Vector2 position, float rotation, float scale)
		: Entity(world, position, rotation, scale)
	{
	}

	void Tank::DoOnBeginPlay()
	{
		String tankTextureName{ "Tank_Right.png"};

		m_renderContext.texturePath = (FileSystem::Get().GetAssetsDir() + tankTextureName);

		ISHAK_LOG(Error, "Tank Created")
	}

	void Tank::DoTick(float deltaTime)
	{
		const float speed{ 10.0f };

		auto velocity{ (Vector2{ 10, 0 }) * (speed * deltaTime)};
		// Moving x amount every frame.
		SetPosition(GetPosition() += velocity);
	}

	void Tank::Fire()
	{
		
	}

}// ishak