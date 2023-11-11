#pragma once

#include "CoreMinimal.h"
#include "EngineConfig.h"

#include "Ecs.h"

namespace ishak
{
	class World;

	class ENGINE_API Level
	{
	public:
		Level(World* world);
		void Init();
	private:
		World* m_world{ nullptr };
		Vector4 m_backGroundColor{ 0, 0, 0, 0 };
		Ecs::EntityId entityId{ Ecs::kNullId };
	};
}// ishak