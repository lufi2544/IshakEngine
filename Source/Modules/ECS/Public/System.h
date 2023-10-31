#pragma once
#include "SystemConfig.h"
#include "ECSConfig.h"
#include "EntityId.h"



namespace ishak::Ecs {


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

		TArray<EntityId> GetSystemEntities() const;
		Signature& GetComponentSignature();

		void AddEntityToSystem(EntityId entityId);
		void RemoveEntityFromSystem(EntityId entityId);

	protected:

		template <typename ComponentT>
		void RequireComponent();


	private:
		/** Bitfield as a requirement check when adding entities to the system, which
		* will have to count with the neccessary components in the signature to be added.*/
		Signature m_signature;

		/**  */
		TArray<EntityId> m_entities;
	};


}// ishak::Ecs