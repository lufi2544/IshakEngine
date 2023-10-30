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

	void Entity::Tick(float deltaTime)
	{		
		DoTick(deltaTime);
	}

	void Entity::Render()
	{
		
	}

	Vector2 Entity::GetPosition() const
	{
		return m_position;
	}

	void Entity::SetPosition(const Vector2& newPosition)
	{
		m_position = newPosition;
	}

	String Entity::GetTexture() const
	{
		return m_renderContext.texturePath;
	}
}// ishak