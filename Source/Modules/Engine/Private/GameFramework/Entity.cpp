#include "GameFramework/Entity.h"
#include "GameFramework/GameInstance.h"
#include "GameFramework/World.h"

//ECS
#include "Ecs//RenderingComponent.h"



namespace ishak {

	Entity::Entity(World* world)
		: m_World(world)
	{		

	}
	Entity::~Entity()
	{


	}
	void Entity::BeginPlay()
	{
		auto ecsHandler{ m_World->GetGameInstance().lock()->GetEcsContext() };

		Ecs::EcsContext* ecsContext{ (*(ecsHandler))[Ecs::ECSContextID::RENDERER].get()};
		entityId = ecsContext->GetEntityManager()->RegisterEntity(this);

		if(bVisible)
		{
			RenderingComponent renderingC;
			renderingC.texturePath = &m_renderContext.texturePath;
			renderingC.position = &m_position;
			 
			ecsContext->GetComponentManipulator()->AddComponentDeferred<RenderingComponent>(entityId, renderingC);
		}

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