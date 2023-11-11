#pragma once

#include "Ecs.h"
#include "CoreMinimal.h"
#include "RendererConfig.h"

namespace ishak
{

	class Renderer;

	class RENDERER_API RenderingSystem : public Ecs::System
	{
	public:
		RenderingSystem(Renderer* renderer);
		

		void SetComponentRequirements() override;
		void Update(float dt, Ecs::EntityId entity) override;

		Renderer* m_wrenderer;
	};
}