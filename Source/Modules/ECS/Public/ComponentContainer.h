#pragma once

#include "CoreMinimal.h"
#include "Component.h"

#include "ECSConfig.h"

namespace ishak { namespace Ecs {

		

		class ECS_API IComponentContainer
		{
		public:
			virtual ~IComponentContainer() = default;			

			virtual std::type_index GetComponentId() = 0;
		};

		template<typename ComponentT>
		class ECS_API ComponentContainer : public IComponentContainer
		{
			static constexpr uint8 INITIAL_COMPONENT_COUNT = 2;
			static constexpr int16 NULL_CONTAINER_IDX = 0xffff;
			
		public:			
			ComponentContainer(uint8 initialComponenentsNum = INITIAL_COMPONENT_COUNT)
			{
				m_components.Reserve(initialComponenentsNum);

				// Fill Free Spaces Indexes Array
				for(size_t idx = 0; idx < initialComponenentsNum; ++idx)
				{
					m_freeSpacesIndexes.Add(idx);
				}

			}

			std::type_index GetComponentId() override
			{
				return std::type_index(typeid(ComponentT));
			}
			
			void AddComponentForEntity(EntityId entity, const ComponentT& component);
			void RemoveComponentForEntity(EntityId entity);
			ComponentT& GetComponent(EntityId entity);
			bool HasComponent(EntityId entity) const;

			TArray<uint16> GetFreeSpaces() const
			{
				return m_freeSpacesIndexes;
			}

			uint16 GetComponentIdxForEntity(EntityId entity, bool& bFound)
			{
				bFound = false;
				auto it{ m_entityComponentIdxMap.find(entity) };
				bFound = it != std::end(m_entityComponentIdxMap);
				if(bFound)
				{
					return m_entityComponentIdxMap[entity];					
				}

				return NULL_CONTAINER_IDX;
			}

			/** Frees the free spaces in the components container and looks for new space in memory that fits the used space */
			void FlushComponentsAllocation();

		private:
			uint16 TryGetFreeIdxForComponent()
			{
				if(m_freeSpacesIndexes.IsEmpty())
				{
					return NULL_CONTAINER_IDX;
				}


				uint16 freeIdx{ m_freeSpacesIndexes.Last() };
				m_freeSpacesIndexes.PopBack();

				return freeIdx;
			}

		private:
			/** Stores the idx for the Entity-ComponentIdx in the componensContainer; */
			std::unordered_map<EntityId, uint16> m_entityComponentIdxMap;

			/** Free spaces in the components container. */
			TArray<uint16> m_freeSpacesIndexes;

			/** Array of components for this type. */
			TArray<ComponentT> m_components;
		};


}
		template<typename ComponentT>
		inline void Ecs::ComponentContainer<ComponentT>::AddComponentForEntity(EntityId entity, const ComponentT& component)
		{
			// TODO Add static assert for the entity, if it matches the platform ptr size, then use a const&(dont't think I am gonna use more than uint32 for the entities)

			// See for free spaces in the array.
			const uint16 freeIdx{ TryGetFreeIdxForComponent() };
			if(freeIdx == NULL_CONTAINER_IDX)
			{
				// Not free space, memory reallocation for the components array.				
				m_components.Add(component);
				m_entityComponentIdxMap[entity] = m_components.Size() - 1;
			}
			else 
			{
				// Found free space
				m_entityComponentIdxMap[entity] = freeIdx;
				m_components[freeIdx] = component;
			}			
		}

		template<typename ComponentT>
		inline void Ecs::ComponentContainer<ComponentT>::RemoveComponentForEntity(EntityId entity)
		{
			auto foundIt{ m_entityComponentIdxMap.find(entity) };
			if(foundIt == std::end(m_entityComponentIdxMap))
			{
				// Not Found

				return;
			}

			m_freeSpacesIndexes.Add(foundIt->second);
			m_entityComponentIdxMap.erase(foundIt);
		}

		template<typename ComponentT>
		inline ComponentT& Ecs::ComponentContainer<ComponentT>::GetComponent(EntityId entity)
		{
			auto foundIt{ m_entityComponentIdxMap.find(entity) };
			if (foundIt == std::end(m_entityComponentIdxMap))
			{
				// TODO Exception
				assert(false);
 			}

			return m_components[foundIt->second];
		}

		template<typename ComponentT>
		inline bool Ecs::ComponentContainer<ComponentT>::HasComponent(EntityId entity) const
		{
			return m_entityComponentIdxMap.find(entity) != std::end(m_entityComponentIdxMap);
		}

		template<typename ComponentT>
		void Ecs::ComponentContainer<ComponentT>::FlushComponentsAllocation()
		{
			if(m_freeSpacesIndexes.IsEmpty())
			{
				return;
			}
			
			const uint16 flushedArraySize{ m_components.Size() - m_freeSpacesIndexes.Size() };
			TArray<ComponentT> newComponentContainer;
			newComponentContainer.Reserve(flushedArraySize);

			for(uint16 idx; idx < m_components.Size(); ++idx)
			{
				if(m_freeSpacesIndexes.Contains(idx))
				{
					// if the current m_components idx is meant to be free then we just skip it
					continue;
				}

				// copy the info from old container to new
				newComponentContainer[idx] = m_components[idx];				
			}

			m_components = newComponentContainer;
		}

}// ishak::Ecs