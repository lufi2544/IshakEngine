#pragma once
#include "CoreMinimal.h"

#include "ECSConfig.h"
#include "EntityId.h"

namespace ishak {
	namespace Ecs {


		typedef uint8 ComponentId;

		template<typename T>
		struct ECS_API Component
		{
		public:
			Component();

			static ComponentId GetId();
			void GetOwner() const { return ownerEntityId; }
			void SetOwner(EntityId entityId)
			{
				ownerEntityId = entityId;
			}

			T& GetData()
			{
				return T;
			}

		protected:
			static ComponentId kIdGenerator;

			EntityId ownerEntityId{ kNullId };
			T data;
		};


		struct HealthData
		{
			int health;
			int maxHealth;
		};



		template<typename T>
		inline Component<T>::Component()
		{
		}

}}// ishak::Ecs