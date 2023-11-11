#pragma once

#include "CoreMinimal.h"
#include "Ecs.h"
#include "EngineConfig.h"

namespace ishak {

	class Renderer;

	class ENGINE_API LevelSystem : public Ecs::System
	{
	public: 		
		LevelSystem(Renderer* renderer);

		void SetComponentRequirements() override;
		void Update(float dt, Ecs::EntityId entity) override;

		Renderer* m_renderer{ nullptr };
	};


}//ishak