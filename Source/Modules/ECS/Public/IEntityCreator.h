#pragma once

#include "ECSConfig.h"

namespace ishak { namespace Ecs{

	class ECS_API IEntityCreator
	{
	public:
		virtual ~IEntityCreator() { };

		virtual EntityId GetEntityId(void* object) = 0;
	};

}}// ishak::Ecs