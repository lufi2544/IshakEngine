#pragma once 

#include "CoreMinimal.h"
#include "Ecs.h"
#include "EngineConfig.h"

namespace ishak
{
	class GlobalEntityCreator : public Ecs::IEntityCreator
	{
	public:
		Ecs::EntityId GetEntityId(void* object) override;


		uint32 m_entityAssignerIdx{ 0 };
	};


}// ishak