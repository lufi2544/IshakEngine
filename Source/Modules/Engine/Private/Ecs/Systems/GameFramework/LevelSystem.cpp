#include "Ecs/Systems/GameFramework/LevelSystem.h"
#include "Ecs/Components/LevelComponent.h"

// Renderer
#include "Renderer.h"


namespace ishak {

	LevelSystem::LevelSystem(Renderer* renderer)
		: m_renderer(renderer)
	{

	}
	void LevelSystem::SetComponentRequirements()
	{
		RequireComponent<LevelComponent>();
	}

	void LevelSystem::Update(float dt, Ecs::EntityId entity)
	{
		LevelComponent& component{ GetComponent<LevelComponent>(entity) };
		
		//m_renderer->QueueRenderCommand(RendererCommand::FromColor(entity, component.color));
	}

}//ishak