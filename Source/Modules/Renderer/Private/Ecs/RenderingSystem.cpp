
#include "Ecs/RenderingSystem.h"
#include "Ecs/RenderingComponent.h"
#include "Ecs/Components/TransformComponent.h"

#include "Log/Logger.h"

#include "Renderer.h"

namespace ishak
{
	RenderingSystem::RenderingSystem(Renderer* renderer)
		: m_wrenderer(renderer)
	{

	}
	void ishak::RenderingSystem::SetComponentRequirements()
	{
		RequireComponent<RenderingComponent>();
		RequireComponent<TransformComponent>();
	}

	void RenderingSystem::Update(float dt, Ecs::EntityId entity)
	{

		if(!m_compManipulator->HasComponent<RenderingComponent>(entity) || !m_compManipulator->HasComponent<TransformComponent>(entity))
		{
			return;
		}

		const RenderingComponent compo{ m_compManipulator->GetComponent<RenderingComponent>(entity) };
		const TransformComponent transform{ m_compManipulator->GetComponent<TransformComponent>(entity) };
		if(!compo.IsValid())
		{
			ISHAK_LOG(Temp, "Entity with Invalid Rendering Component.")
			return;
		}

		
		m_wrenderer->QueueRenderCommand(RendererCommand::FromTexture(entity, *compo.texturePath, transform.position));
	}
}// ishak

