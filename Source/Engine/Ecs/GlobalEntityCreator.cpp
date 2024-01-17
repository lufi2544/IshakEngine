#include "Engine/Ecs/GlobalEntityCreator.h"

namespace ishak
{

	Ecs::EntityId GlobalEntityCreator::GetEntityId(void* object)
	{
		if (!object)
		{
			return Ecs::kNullId;
		}

		return ++m_entityAssignerIdx;
	}

}// ishak