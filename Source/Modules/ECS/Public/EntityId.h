#pragma once

#include "ECSConfig.h"
#include "CoreMinimal.h"

namespace ishak { namespace Ecs {

		typedef uint16 EntityId;

		constexpr EntityId kNullId = std::numeric_limits<EntityId>::max();

} }// ishak::ECS