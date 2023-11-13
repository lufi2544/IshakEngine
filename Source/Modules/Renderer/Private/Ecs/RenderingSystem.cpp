
#include "Ecs/RenderingSystem.h"
#include "Ecs/RenderingComponent.h"

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
	}

	void RenderingSystem::Update(float dt, Ecs::EntityId entity)
	{
		const RenderingComponent compo{ m_compManipulator->GetComponent<RenderingComponent>(entity) };
		if(!compo.IsValid())
		{
			ISHAK_LOG(Temp, "Entity with Invalid Rendering Component.")
			return;
		}

		auto t{ *compo.texturePath };
		auto p{ *compo.position };
		
		m_wrenderer->QueueRenderCommand(RendererCommand::FromTexture(entity, t, p));
	}
}// ishak

