#pragma once 

// Core
#include "CoreMinimal.h"

// Ecs
#include "ECSConfig.h"
#include "EntityId.h"
#include "System.h"
#include "Component.h"
#include "ComponentContainer.h"

// STD
#include <typeindex>

namespace ishak { namespace Ecs {

		/** Holds info about the entities to execute the systems on, along with the respective components container. */
		class ECS_API ComponentManipulator
		{
			typedef std::type_index ContainerIdT;
		public:
			ComponentManipulator() = default;

			void RegisterSystem(const SharedPtr<ISystem>& system);
			void RegisterComponentContainer(const SharedPtr<IComponentContainer>& container);			

			template<class ComponentT>
			auto Debug_GetFreeSpacesIndexes()-> decltype(auto)
			{
				auto foundContainer{ m_componentContainers.find(std::type_index(typeid(ComponentT))) };
				if (foundContainer == std::end(m_componentContainers))
				{
					// TODO Exception
					assert(false);

				}

				ComponentContainer<ComponentT>* castedContainer =
					dynamic_cast<ComponentContainer<ComponentT>*>(foundContainer->second.get());

				if (!castedContainer)
				{
					assert(false);
				}

				return castedContainer->GetFreeSpaces();
			}

			template<class ComponentT>
			auto Debug_GetComponentIdxForEntity(EntityId entity, bool& bFound) -> decltype(auto)
			{
				auto foundContainer{ m_componentContainers.find(std::type_index(typeid(ComponentT))) };
				if (foundContainer == std::end(m_componentContainers))
				{
					// TODO Exception
					assert(false);

				}

				ComponentContainer<ComponentT>* castedContainer =
					dynamic_cast<ComponentContainer<ComponentT>*>(foundContainer->second.get());

				if (!castedContainer)
				{
					assert(false);
				}

				return castedContainer->GetComponentIdxForEntity(entity, bFound);
			}

			template<class ComponentT>
			void AddComponent(EntityId entity, const ComponentT& component)
			{				
				auto foundContainer{ m_componentContainers.find(std::type_index(typeid(component))) };
				if(foundContainer == std::end(m_componentContainers))
				{
					// TODO Exception
					return;
				}

				ComponentContainer<ComponentT>* castedContainer = 
					dynamic_cast<ComponentContainer<ComponentT>*>(foundContainer->second.get());

				if(!castedContainer)
				{
					return;
				}					
				castedContainer->AddComponentForEntity(entity, component);

				//TODO Figure out the id for every componentT.
				m_entitiesSignature[entity].set(1);
			}

			template<class ComponentT>
			void RemoveComponent(EntityId entity)
			{
				auto foundContainer{ m_componentContainers.find(std::type_index(typeid(ComponentT))) };
				if (foundContainer == std::end(m_componentContainers))
				{
					// TODO Exception
					assert(false);

				}

				ComponentContainer<ComponentT>* castedContainer =
					dynamic_cast<ComponentContainer<ComponentT>*>(foundContainer->second.get());

				if (!castedContainer)
				{
					assert(false);
				}

				castedContainer->RemoveComponentForEntity(entity);
			}

			template<class ComponentT>
			ComponentT& GetComponent(EntityId entity)
			{
				auto foundContainer{ m_componentContainers.find(std::type_index(typeid(ComponentT))) };
				if (foundContainer == std::end(m_componentContainers))
				{
					// TODO Exception
					assert(false);
					
				}

				ComponentContainer<ComponentT>* castedContainer = 
					dynamic_cast<ComponentContainer<ComponentT>*>(foundContainer->second.get());

				if (!castedContainer)
				{
					assert(false);
				}

				return castedContainer->GetComponent(entity);
			}

			template<class ComponentT>
			bool HasComponent(EntityId entity)
			{
				auto foundContainer{ m_componentContainers.find(std::type_index(typeid(ComponentT))) };
				if (foundContainer == std::end(m_componentContainers))
				{
					// TODO Exception
					assert(false);
					return false;
				}

				ComponentContainer<ComponentT>* castedContainer = 
					dynamic_cast<ComponentContainer<ComponentT>*>(foundContainer->second.get());

				if (!castedContainer)
				{
					assert(false);
					return false;
				}

				return castedContainer->HasComponent(entity);
			}

			void UpdateSystems(float deltaTime);
			
			

			/** World entities. */
			TArray<EntityId> m_entities;
			
			/** Containers of Components, contiguous in memory. */
			std::unordered_map<ContainerIdT, SharedPtr<IComponentContainer>> m_componentContainers;

			/** Engine registered systems. */
			TArray<SharedPtr<ISystem>> m_systems;

			/** Entities signature. This is used for checking if the entiteis have certain signature for upating the systems. */
			std::unordered_map<EntityId, Signature> m_entitiesSignature;
		};


	}
}// ishak::Ecs