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


		struct ComponentContainerInfo
		{
			ComponentContainerInfo() = default;

			SharedPtr<IComponentContainer> container;
			uint8 componentSignature;
		};

		/** Global Component Container that manages the entire compoenents for a certain type. All the collection of components is mapped to a certain entity
		* so when retreiving a compoenent for an entity, the retreival is instantanious.
		* 
		* Collection of components is contigus in memory, we reserve the space for certain amount of components, default components are created. When a component is requested to 
		* be added to an entity, a free component from the collection is retreived and mapped to that entity.
		* 
		* **NOTE:This class also gives us the possibility to Flush the components collection, that means that we can release the free spaces in the components collection and allocate
		* the components again in a new space in memory with the current used size of the components. 
		* 
		* E.g: 
		* 
		* -> Before Flush:
		* 
		*	Current Assigned Components: 200 || CollectionCapacityAndSize: 300 (the ones that are free are defaulted)
		*   Free Components: 100			 || Free Components Capacity: 300
		* 
		* -> After Flush:
		* 
		*   Current Assigned Components: 100 || CollectionCapacityAndSize: 100 (the ones that are free are defaulted)
		*   Free Components: 0			 || Free Components Capacity: 0
		* 
		* **IMPORTANT: TODO, when flushed or added components and no free spaces are available, then the collection grows doubleing the size due to the TArray logic. Add Custom Allocator
		*  so every time we grow in size, we do not do this. Maybe with just a custom TArray would be okay?
		*/
		template<typename ComponentT>
		class ECS_API ComponentContainer : public IComponentContainer
		{
			typedef uint16 CollectionIndexT;
			static constexpr uint8 INITIAL_COMPONENT_COUNT = 2;
			static constexpr CollectionIndexT NULL_CONTAINER_IDX = 0xffff;
			
		public:			
			ComponentContainer(uint8 initialComponenentsNum = INITIAL_COMPONENT_COUNT)
			{
				if(initialComponenentsNum == 0)
				{
					initialComponenentsNum = INITIAL_COMPONENT_COUNT;
				}

				m_components.Reserve(initialComponenentsNum);
				m_components.AddDefaulted(initialComponenentsNum);
				
				AllocateFreeSpacesIndexes(0, m_components.Capacity() - 1);
			}

			std::type_index GetComponentId() override
			{
				return std::type_index(typeid(ComponentT));
			}

			TArray<ComponentT>* GetComponentsCollection()
			{
				return &m_components;
			}
			
			void AddComponentForEntity(EntityId entity, const ComponentT& component);
			void RemoveComponentForEntity(EntityId entity);
			ComponentT& GetComponent(EntityId entity);
			bool HasComponent(EntityId entity) const;

			TArray<CollectionIndexT> GetFreeSpaces() const
			{
				return m_freeSpacesIndexes;
			}

			CollectionIndexT GetComponentIdxForEntity(EntityId entity, bool& bFound)
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

			/** Frees the free spaces in the components container and looks for new space in memory that fits the used space. */
			void FlushComponentsAllocation();

		private:

			void AllocateFreeSpacesIndexes(CollectionIndexT firstIdx, CollectionIndexT lastIdx)
			{
				m_freeSpacesIndexes.Reserve(m_components.Capacity());
				// Fill Free Spaces Indexes Array
				for (CollectionIndexT idx = firstIdx; idx <= lastIdx; ++idx)
				{
					m_freeSpacesIndexes.Add(idx);
				}
			}



			CollectionIndexT TryGetFreeIdxForComponent()
			{
				if(m_freeSpacesIndexes.IsEmpty())
				{
					return NULL_CONTAINER_IDX;
				}


				CollectionIndexT freeIdx{ m_freeSpacesIndexes.Last() };
				m_freeSpacesIndexes.PopBack();

				return freeIdx;
			}

		private: 
			void ReleaseNonOccupiedComponentsAndReallocateUsedOnes();
			void RemapEntityToCollectionIndexMap(CollectionIndexT oldIdx, CollectionIndexT* newIdx);

		private:
			/** Stores the idx for the Entity-ComponentIdx in the componensContainer; */
			std::unordered_map<EntityId, CollectionIndexT> m_entityComponentIdxMap;

			/** Free spaces in the components container. */
			TArray<CollectionIndexT> m_freeSpacesIndexes;

			/** Array of components for this type. */
			TArray<ComponentT> m_components;
		};


}
		template<typename ComponentT>
		inline void Ecs::ComponentContainer<ComponentT>::AddComponentForEntity(EntityId entity, const ComponentT& component)
		{
			// TODO Add static assert for the entity, if it matches the platform ptr size, then use a const&(dont't think I am gonna use more than uint32 for the entities)

			// See for free spaces in the array.
			const CollectionIndexT freeIdx{ TryGetFreeIdxForComponent() };
			if(freeIdx == NULL_CONTAINER_IDX)
			{
				// TODO Add TESTS for retreiving entity after this resize happens.
				// Not free space, memory reallocation for the components array.	
				const uint32 lastComponentsCollectionCapacity{ m_components.Capacity() };

				// New allocation for the array done. Need this so the array grows and capacity is updated
				m_components.AddDefaulted(1);				

				const uint32 currentComponentsCollectionCapacity{ m_components.Capacity() };

				// Fill the new free components in the collection after reallocating memory.
				m_components.AddDefaulted(m_components.Capacity() - m_components.Size());

				// As we have already added one component, we have to start flagging as free one idx further.
				const uint32 firstIdxToFlagAsFree{ lastComponentsCollectionCapacity };
				const uint32 lastIdxToFlagAsFree{ currentComponentsCollectionCapacity - 1 };
				AllocateFreeSpacesIndexes(firstIdxToFlagAsFree, lastIdxToFlagAsFree);
				
				const CollectionIndexT entityComponentIdx{TryGetFreeIdxForComponent()};				
				m_components[entityComponentIdx] = component;

				assert(entityComponentIdx != NULL_CONTAINER_IDX);

				m_entityComponentIdxMap[entity] = entityComponentIdx;
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
			
			ReleaseNonOccupiedComponentsAndReallocateUsedOnes();
		}

		template<typename ComponentT>
		inline void Ecs::ComponentContainer<ComponentT>::ReleaseNonOccupiedComponentsAndReallocateUsedOnes()
		{
			const auto flushedArraySize{ m_components.Size() - m_freeSpacesIndexes.Size() };
			TArray<ComponentT> newComponentContainer;
			newComponentContainer.Reserve(flushedArraySize);
			newComponentContainer.AddDefaulted(flushedArraySize);

			CollectionIndexT newContainerIdx = 0;
			for (CollectionIndexT idx = 0; idx < m_components.Size(); ++idx)
			{
				// if the current m_components idx is meant to be free then we just skip it
				if (m_freeSpacesIndexes.Contains(idx))
				{
					continue;
				}

				// copy the info from old container to new
				newComponentContainer[newContainerIdx] = m_components[idx];

				// remap the index in the indexToEntity map.
				RemapEntityToCollectionIndexMap(idx, &newContainerIdx);

			}

			assert(newComponentContainer.Size() == flushedArraySize);
			assert(newComponentContainer.Capacity() == flushedArraySize);

			m_freeSpacesIndexes.Clear();
			m_components = newComponentContainer;
		}


		template<typename ComponentT>
		inline void Ecs::ComponentContainer<ComponentT>::RemapEntityToCollectionIndexMap(CollectionIndexT oldIdx, CollectionIndexT* newIdx)
		{
			// remap the index in the indexToEntity map.
			for (auto& [entity, collectionIdx] : m_entityComponentIdxMap)
			{

				if (collectionIdx == oldIdx)
				{
					// remap the idx to the new collection.
					collectionIdx = (*(newIdx))++;

					break;
				}
			}
		}

}// ishak::Ecs