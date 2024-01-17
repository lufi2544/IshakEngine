#pragma once

#include "ComponentManipulator.h"
#include "IEntityCreator.h"
#include "Core/CoreMinimal.h"
#include "EntityId.h"

#include "ECSConfig.h"

namespace ishak { namespace Ecs {

	/**  
	* Class that handles entity registration/unregistration in Engine.
	*/
	class ECS_API EntityManager
	{
	public:
		EntityManager() = default;
		EntityManager(UniquePtr<IEntityCreator>&& entityCreator);

		EntityManager(EntityManager const&) = delete;
		EntityManager(EntityManager&&) = delete;
		
		EntityId RegisterEntity(void* entityObject);
		EntityId GetEntityId(void* ptr);
		EntityId UnregisterEntity(void* entityObject);

		TArray<EntityId>& GetEntitiesCollection();		
		
	private:		
		TArray<EntityId> m_entities;
		SharedPtr<IEntityCreator> m_entityCreator;		

		std::unordered_map<void*, EntityId> m_entityMap;
	};

} }// ishak::Ecs