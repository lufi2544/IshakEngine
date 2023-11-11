#include "EntityManager.h"

namespace ishak { namespace Ecs {


	EntityManager::EntityManager(UniquePtr<IEntityCreator>&& entityCreator)
		: m_entityCreator(std::move(entityCreator))
	{

	}

	EntityId EntityManager::RegisterEntity(void* entityObject)
	{
		if(!entityObject || !m_entityCreator)
		{
			return kNullId;
		}

		auto foundIt{ m_entityMap.find(entityObject) };

		if(foundIt != std::end(m_entityMap))
		{						
			// entity already registered
			return foundIt->second;
		}

		const EntityId createdEntity{ m_entityCreator->GetEntityId(entityObject) };		
		m_entityMap[entityObject] = createdEntity;
		m_entities.Add(createdEntity);

		return createdEntity;
	}

	EntityId EntityManager::GetEntityId(void* ptr)
	{
		if (!ptr || !m_entityCreator)
		{
			return kNullId;
		}

		auto foundIt{ m_entityMap.find(ptr) };

		if (foundIt != std::end(m_entityMap))
		{
			// entity already registered
			return foundIt->second;
		}

		return kNullId;
	}

	EntityId EntityManager::UnregisterEntity(void* entityObject)
	{
		auto foundIt{ m_entityMap.find(entityObject) };

		if(foundIt == std::end(m_entityMap))
		{
			// entity not found
			return kNullId;
		}		
		const EntityId cachedEntityFound{ foundIt->second };

		m_entityMap.erase(foundIt);
		return cachedEntityFound;
	}

	TArray<EntityId>& EntityManager::GetEntitiesCollection()
	{
		return m_entities;
	}



}}// ishak::Ecs