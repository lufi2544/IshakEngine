#include "ECS/GameEntityCreator.h"

namespace ishak
{
	Ecs::EntityId GameEntityCreator::GetEntityId(void* object)
	{
		if(!object)
		{
			return Ecs::kNullId;
		}

		return ++m_entityAssignerIdx;		
	}
}// ishak