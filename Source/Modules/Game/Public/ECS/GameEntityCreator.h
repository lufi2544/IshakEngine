#pragma once

#include "CoreMinimal.h"
#include "EntityId.h"
#include "IEntityCreator.h"

namespace ishak
{
	class GameEntityCreator : public Ecs::IEntityCreator
	{
	public:
		GameEntityCreator() = default;

		Ecs::EntityId GetEntityId(void* object) override;


		uint32 m_entityAssignerIdx{ 0 };
	};
	

}// ishak