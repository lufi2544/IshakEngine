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

			// TODO Maybe chaning this to another place EcsContext?
			void RegisterEntity(EntityId entity);
			void RegisterSystem(SharedPtr<System>&& system);
			void RegisterComponentContainer(SharedPtr<IComponentContainer>&& container);

			template<class ComponentT>
			uint8 GetComponentContainerSignatureId();

			template<class ComponentT>
			void AddComponent(EntityId entity, const ComponentT& component);

			template<class ComponentT>
			void RemoveComponent(EntityId entity);

			template<class ComponentT>
			ComponentT& GetComponent(EntityId entity);

			template<class ComponentT>
			bool HasComponent(EntityId entity);

			void UpdateSystems(float deltaTime);

			template<class ComponentT>
			void FlushComponenContainerAllocation();


			template<class ComponentT>
			auto Debug_GetFreeSpacesIndexes() -> decltype(auto);

			template<class ComponentT>
			auto Debug_GetComponentIdxForEntity(EntityId entity, bool& bFound) -> decltype(auto);

			template<class ComponentT>
			auto Debug_GetComponentsCollection() -> decltype(auto);
			
			
		private:
			template<class ComponentT>
			ComponentContainer<ComponentT>* GetCastedComponent();

			template<class ComponentT>
			ComponentContainerInfo GetComponentContainerInfo()
			{
				auto foundContainer{ m_componentContainers.find(std::type_index(typeid(ComponentT))) };
				if (foundContainer == std::end(m_componentContainers))
				{
					// TODO Exception
					assert(false);
					return { };
				}

				return foundContainer->second;
			}

		private:
			/** World entities. */
			TArray<EntityId> m_entities; 			

			/** Containers of Components, contiguous in memory. */
			std::unordered_map<ContainerIdT,  ComponentContainerInfo> m_componentContainers;

			/** Engine registered systems. */
			TArray<SharedPtr<System>> m_systems;

			/** Entities signature. This is used for checking if the entiteis have certain signature for upating the systems. */
			std::unordered_map<EntityId, Signature> m_entitiesSignature;					
		};


		template<class ComponentT>
		inline uint8 ComponentManipulator::GetComponentContainerSignatureId()
		{
			const ComponentContainerInfo info{ GetComponentContainerInfo<ComponentT>() };
			return info.componentSignature;
		}

		template<class ComponentT>
		inline ComponentContainer<ComponentT>* ComponentManipulator::GetCastedComponent()
		{
			auto foundContainer{ m_componentContainers.find(std::type_index(typeid(ComponentT))) };
			if (foundContainer == std::end(m_componentContainers))
			{
				// TODO Exception
				assert(false);
				return nullptr;
			}

			ComponentContainer<ComponentT>* castedContainer =
				dynamic_cast<ComponentContainer<ComponentT>*>(foundContainer->second.container.get());

			if (!castedContainer)
			{
				assert(false);
				return nullptr;
			}

			return castedContainer;
		}		

		template<class ComponentT>
		inline void ComponentManipulator::AddComponent(EntityId entity, const ComponentT& component)
		{
			auto castedContainer{ GetCastedComponent<ComponentT>() };

			castedContainer->AddComponentForEntity(entity, component);
			
			ComponentContainerInfo info{ GetComponentContainerInfo<ComponentT>() };
			m_entitiesSignature[entity].set(info.componentSignature);
		}

		template<class ComponentT>
		inline void ComponentManipulator::RemoveComponent(EntityId entity)
		{
			auto castedContainer{ GetCastedComponent<ComponentT>() };

			castedContainer->RemoveComponentForEntity(entity);
		}

		template<class ComponentT>
		inline ComponentT& ComponentManipulator::GetComponent(EntityId entity)
		{
			auto castedContainer{ GetCastedComponent<ComponentT>() };

			return castedContainer->GetComponent(entity);
		}

		template<class ComponentT>
		inline bool ComponentManipulator::HasComponent(EntityId entity)
		{
			auto castedContainer{ GetCastedComponent<ComponentT>() };

			return castedContainer->HasComponent(entity);
		}

		template<class ComponentT>
		inline void ComponentManipulator::FlushComponenContainerAllocation()
		{
			auto castedContainer{ GetCastedComponent<ComponentT>() };

			castedContainer->FlushComponentsAllocation();
		}

		template<class ComponentT>
		inline auto ComponentManipulator::Debug_GetFreeSpacesIndexes() -> decltype(auto)
		{
			auto castedContainer{ GetCastedComponent<ComponentT>() };

			return castedContainer->GetFreeSpaces();
		}

		template<class ComponentT>
		inline auto ComponentManipulator::Debug_GetComponentIdxForEntity(EntityId entity, bool& bFound) -> decltype(auto)
		{
			auto castedContainer{ GetCastedComponent<ComponentT>() };

			return castedContainer->GetComponentIdxForEntity(entity, bFound);
		}

		template<class ComponentT>
		inline auto ComponentManipulator::Debug_GetComponentsCollection() -> decltype(auto)
		{
			auto castedContainer{ GetCastedComponent<ComponentT>() };

			return castedContainer->GetComponentsCollection();
		}


}
}// ishak::Ecs