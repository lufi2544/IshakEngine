#pragma once

#include "SystemConfig.h"
#include "ECSConfig.h"
#include "EntityId.h"



namespace ishak { namespace Ecs {


		class ComponentManipulator;

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

			virtual void SetRequirements() { };

			template <typename ComponentT>
			void RequireComponent();


		protected:
			/** Bitfield as a requirement check when adding entities to the system, which
			* will have to count with the neccessary components in the signature to be added.*/
			Signature m_signature;

			ComponentManipulator* m_compManipulator{ nullptr };
		};


		struct TransformComponent
		{
			int x, y, z;
		};

		class ECS_API HealthSystem : public System
		{
		public:
			HealthSystem();
			
			void Update(float dt, EntityId entity) override;

		protected:
			void SetRequirements() override;
		};

	}
}// ishak::Ecs