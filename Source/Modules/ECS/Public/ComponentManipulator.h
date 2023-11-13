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
		/**
		* Concers for the cache: 
		* -> Copying the components when adding components commands to the queue buffer.
		* -> Iterating through the systems and then had to get the entity idx, maybe batch the entities and components for less cache misses.
		*/
		class ECS_API ComponentManipulator
		{
			typedef std::type_index ContainerIdT;
			typedef std::function<void(void)> ComponentQueueCommandT;
			typedef std::unordered_map<EntityId, Signature> SignaturesMapT;
		public:
			ComponentManipulator() = default;

			SignaturesMapT& GetSignaturesMap()
			{
				return m_entitiesSignaturesMap;
			}

			Signature GetEntitySignature(EntityId entity, bool& bFound);
			void UnregisterEntitySignature(EntityId entity);

			template<typename... ComponentC>
			void BatchEntitiesComponents(std::unordered_map<EntityId, std::tuple<ComponentC...>>* comps, const Signature& forSignature)
			{								
				for(auto signature : m_entitiesSignaturesMap)
				{
					// if entity has the signature
					if(!(signature.second & forSignature).any())
					{
						continue;
					}

					std::tuple<ComponentC...> componentTuple;

					EntityId entityId = signature.first;
					// Use fold expression to apply a function to each component type
					// Here we create an object of every template Arg and apply the lambda to it
					([&](auto comp) {
		
							// Get the component and add it to the tuple
							auto& component = GetComponent<decltype(comp)>(entityId);
							std::get<decltype(comp)>(componentTuple) = component;
						

						}(ComponentC{ }), ...);

					comps->insert(std::make_pair(entityId, componentTuple));
				}

					
				//assert(ids->Size() == comps->Size());				
			}

			// TODO Maybe chaning this to another place EcsContext?						
			void RegisterComponentContainer(SharedPtr<IComponentContainer>&& container);
			void UpdateComponentsStates();

			template<class ComponentT>
			uint8 GetComponentContainerSignatureId();

			template<class ComponentT>
			void AddComponentDeferred(EntityId entity, const ComponentT& component);

			template<class ComponentT>
			void RemoveComponentDeferred(EntityId entity);

			template<class ComponentT>
			ComponentT& GetComponent(EntityId entity);

			template<class ComponentT>
			bool HasComponent(EntityId entity);			

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
			/** Containers of Components, contiguous in memory. */
			std::unordered_map<ContainerIdT,  ComponentContainerInfo> m_componentContainers;
						
			/** Entities signature. This is used for checking if the entiteis have certain signature for upating the systems. */
			SignaturesMapT m_entitiesSignaturesMap;

			/** Queue of commands that will change the state of a component Add/Remove at the end of the frame. */
			TArray<ComponentQueueCommandT> m_commandQueue;
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
		inline void ComponentManipulator::AddComponentDeferred(EntityId entity, const ComponentT& component)
		{
			// TODO Safe Memory here
			ComponentQueueCommandT command = [this, entity, component]()
			{
				auto castedContainer{ GetCastedComponent<ComponentT>() };

				castedContainer->AddComponentForEntity(entity, component);
			
				ComponentContainerInfo info{ GetComponentContainerInfo<ComponentT>() };
				m_entitiesSignaturesMap[entity].set(info.componentSignature);
			};

			m_commandQueue.Add(command);
		}

		template<class ComponentT>
		inline void ComponentManipulator::RemoveComponentDeferred(EntityId entity)
		{			
			ComponentQueueCommandT command = [this, entity]()
			{
				auto castedContainer{ GetCastedComponent<ComponentT>() };
				const uint8 containerSignature = GetComponentContainerSignatureId<ComponentT>();
				
				// Unset the signature 
				m_entitiesSignaturesMap[entity].set(containerSignature, false);

				castedContainer->RemoveComponentForEntity(entity);
			};

			m_commandQueue.Add(command);
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