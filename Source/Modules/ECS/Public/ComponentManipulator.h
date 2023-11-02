#pragma once 

// Core
#include "CoreMinimal.h"

// Ecs
#include "ECSConfig.h"
#include "EntityId.h"
#include "System.h"
#include "Component.h"
#include "ComponentContainer.h"

namespace ishak { namespace Ecs {

		/** Holds info about the entities to execute the systems on, along with the respective components container. */
		class ECS_API ComponentManipulator
		{
		public:
			ComponentManipulator() = default;

			void RegisterSystem(const ISystem& system);
			void RegisterComponentContainer(const SharedPtr<IComponentContainer>& container);

			template<class ComponentT>
			void AddComponent(EntityId entity, const ComponentT& component)
			{
				component.SetOwner(entity);
				auto container = reinterpret_cast<ComponentContainer<ComponentT>*>(m_componentContainers[component->GetId()].get());
				if (container == nullptr)
				{
					// TODO Exception
					return;
				}
				container->components.Add(component);
			}

			void UpdateSystems();

			/** World entities. */
			TArray<EntityId> m_entities;

			// TODO Create Map Container.
			/** Containers of Components, contiguous in memory. */
			std::map<ComponentId, SharedPtr<IComponentContainer>> m_componentContainers;

			/** Engine registered systems. */
			TArray<ISystem> m_systems;
		};


	}
}// ishak::Ecs