#pragma once

#include "ECSConfig.h"
#include "EntityId.h"

namespace ishak { namespace Ecs{

	/** Class in charge of creating an EntityId. Should be subclassed for custom assignation. */
	class ECS_API IEntityCreator
	{
	public:
		virtual ~IEntityCreator() { };

		virtual EntityId GetEntityId(void* object) = 0;
	};

}}// ishak::Ecs