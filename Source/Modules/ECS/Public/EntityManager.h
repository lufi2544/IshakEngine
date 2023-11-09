#pragma once

#include "ComponentManipulator.h"
#include "IEntityCreator.h"
#include "CoreMinimal.h"
#include "EntityId.h"

#include "ECSConfig.h"

namespace ishak { namespace Ecs {

	class ECS_API EntityManager
	{
	public:
		EntityManager() = default;
		EntityManager(UniquePtr<IEntityCreator>&& entityCreator);

		EntityManager(EntityManager const&) = delete;
		EntityManager(EntityManager&&) = delete;
		
		EntityId RegisterEntity(void* entityObject);
		EntityId GetEntityId(void* ptr);
		void UnregisterEntity(void* entityObject);

		TArray<EntityId>& GetEntitiesCollection();		
		
	private:		
		TArray<EntityId> m_entities;
		UniquePtr<IEntityCreator> m_entityCreator;		

		std::unordered_map<void*, EntityId> m_entityMap;
	};

} }// ishak::Ecs