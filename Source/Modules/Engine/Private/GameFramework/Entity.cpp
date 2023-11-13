#include "GameFramework/Entity.h"
#include "GameFramework/GameInstance.h"
#include "GameFramework/World.h"

//ECS
#include "Ecs//RenderingComponent.h"
#include <Ecs/Components/TransformComponent.h>



namespace ishak {

	Entity::Entity(World* world)
		: m_World(world)
	{		

	}

	Entity::Entity(World* world, Vector2 position)
		: m_World(world)
		, m_position(position)
	{

	}

	Entity::~Entity()
	{


	}
	void Entity::BeginPlay()
	{		
		Ecs::EcsContext* ecsContext{ m_World->GetGameInstance().lock()->GetEcsContext(Ecs::ContextID::RENDERER) };
		entityId = ecsContext->GetEntityManager()->RegisterEntity(this);

		if(bVisible)
		{
			RenderingComponent renderingC;
			renderingC.texturePath = &m_renderContext.texturePath;			
			 
			ecsContext->GetComponentManipulator()->AddComponentDeferred<RenderingComponent>(entityId, renderingC);
		}
		
		GetWorld()->GetGameInstance().lock()->GetEcsContext(Ecs::ContextID::RENDERER)->GetComponentManipulator()->AddComponentDeferred(entityId, TransformComponent{ m_position });

		DoOnBeginPlay();
	}

	void Entity::Tick(float deltaTime)
	{		
		DoTick(deltaTime);
	}

	void Entity::Render()
	{
		
	}

	World* Entity::GetWorld()
	{
		return m_World;
	}

	Vector2 Entity::GetPosition()
	{

		Ecs::ComponentManipulator* compManipulator{ GetWorld()->GetGameInstance().lock()->GetEcsContext(Ecs::ContextID::ENGINE)->GetComponentManipulator() };
		if (!compManipulator)
		{
			return{ };
		}

		if (!compManipulator->HasComponent<TransformComponent>(entityId))
		{
			return{ };
		}

		auto transform{ GetWorld()->GetGameInstance().lock()->GetEcsContext(Ecs::ContextID::ENGINE)->GetComponentManipulator()->GetComponent<TransformComponent>(entityId) };

		return transform.position;
	}

	void Entity::SetPosition(const Vector2& newPosition)
	{
		Ecs::ComponentManipulator* compManipulator{ GetWorld()->GetGameInstance().lock()->GetEcsContext(Ecs::ContextID::ENGINE)->GetComponentManipulator() };
		if(!compManipulator)
		{
			return;
		}

		if(!compManipulator->HasComponent<TransformComponent>(entityId))
		{
			return;
		}

		TransformComponent& transform{ compManipulator->GetComponent<TransformComponent>(entityId) };
		transform.position = newPosition;
	}

	String Entity::GetTexture() const
	{
		return m_renderContext.texturePath;
	}
}// ishak