#include "Engine/GameFramework/Entity.h"
#include "Engine/GameFramework/GameInstance.h"
#include "Engine/GameFramework/World.h"

//Rendering
#include "Renderer/RendererTypes.h"

//ECS
#include "Core/Ecs/Components/TransformComponent.h"
#include "Renderer/Ecs//RenderingComponent.h"
#include "Renderer/Ecs/Components/TextureComponent.h"



namespace ishak {

	Entity::Entity(World* world)
		: m_World(world)
	{		

	}

	Entity::Entity(World* world, Vector2 position, float rotation, float scale)
		: m_World(world)
		, m_position(position)
		, m_rotation(rotation)
		, m_scale(scale)
	{

	}

	Entity::~Entity()
	{


	}
	void Entity::BeginPlay()
	{		
		Ecs::EcsContext* ecsContext{ m_World->GetGameInstance().lock()->GetEcsContext(Ecs::ContextID::RENDERER) };
		entityId = ecsContext->GetEntityManager()->RegisterEntity(this);

		SetPositionAndRotationAndScale(m_position, m_rotation, m_scale);

		if(bVisible)
		{
			TextureComponent textureC;
			textureC.texturePath = &m_renderContext.texturePath;
			textureC.ZOrder = (uint8)ERendererLayers::Player;
			 
			ecsContext->GetComponentManipulator()->AddComponentDeferred<>(entityId, textureC);
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

	void Entity::SetPositionAndRotationAndScale(const Vector2& newPosition, const float& newRotation, const float& newScaleParam)
	{
		m_scale = newScaleParam;
		m_position = newPosition;
		m_rotation = newRotation;
		Ecs::ComponentManipulator* compManipulator{ GetWorld()->GetGameInstance().lock()->GetEcsContext(Ecs::ContextID::ENGINE)->GetComponentManipulator() };
		if (!compManipulator)
		{
			return;
		}

		if (!compManipulator->HasComponent<TransformComponent>(entityId))
		{
			// Add Compoonent
			GetWorld()->GetGameInstance().lock()->GetEcsContext(Ecs::ContextID::ENGINE)->GetComponentManipulator()->AddComponentDeferred(entityId, TransformComponent{ newPosition, newRotation, newScaleParam });
			return;
		}

		TransformComponent& transform{ compManipulator->GetComponent<TransformComponent>(entityId) };
		transform.position = newPosition;
		transform.rotation = newRotation;
	}

	String Entity::GetTexture() const
	{
		return m_renderContext.texturePath;
	}
}// ishak