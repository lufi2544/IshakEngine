#pragma once
#include "CoreMinimal.h"

#include "ECSConfig.h"
#include "EntityId.h"
#include <typeinfo>

namespace ishak {
	namespace Ecs {
		

		
		struct ECS_API Component
		{
		public:
			Component() = default;			
			EntityId GetOwner() const { return ownerEntityId; }
			void SetOwner(EntityId entityId)
			{
				ownerEntityId = entityId;
			}



		protected:			
			EntityId ownerEntityId{ kNullId };			
		};


		struct HealthData
		{
			int health;
			int maxHealth;
		};


}}// ishak::Ecs