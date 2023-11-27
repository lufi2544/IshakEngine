#pragma once

#include "SystemConfig.h"
#include "ECSConfig.h"
#include "EntityId.h"
#include "ComponentManipulator.h"

// STD
#include <tuple>

#include <array>

namespace ishak { namespace Ecs {
	
		/**
		* Base class for the ECS Systems. This will Udpate every frame if there are entities in the world
		* that contain the Required component types from the Signature of the System.
		*
		* If we have a damage system, we would Update the system only if there are entities in the world
		* which have the DamageComponent and the HealthComponent for example.
		*
		* This is an entity based desing, means that the entities are stored and updated,
		* I would like to store the components itself, so we can pass them to the system directly.
		*/
		class ECS_API System
		{
		public:
			System();

			void SetComponentManipulator(ComponentManipulator* compManipulator);
			virtual void Update(float dt, EntityId entity) { };

			Signature& GetSignature();			

		protected:

			template<typename ComponentT>
			ComponentT& GetComponent(EntityId entity)
			{
				return m_compManipulator->GetComponent<ComponentT>(entity);
			}

			virtual void SetComponentRequirements() { };

			template <typename ComponentT>
			void RequireComponent()
			{
				if (!m_compManipulator)
				{
					assert(false);
					return;
				}
				m_signature.set(m_compManipulator->GetComponentContainerSignatureId<ComponentT>());
			}


		protected:
			/** Bitfield as a requirement check when adding entities to the system, which
			* will have to count with the neccessary components in the signature to be added.*/
			Signature m_signature;

			ComponentManipulator* m_compManipulator{ nullptr };
		};
	}
}// ishak::Ecs
