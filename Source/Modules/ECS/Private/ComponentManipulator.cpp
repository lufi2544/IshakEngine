#include "ComponentManipulator.h"



namespace ishak::Ecs
{
	void ComponentManipulator::RegisterEntity(EntityId entity)
	{
		
		// TODO Refactor this to an object that is in charge of giving the EntityIds.
		if(m_entities.Contains(entity))
		{
			// Entity already exists
			assert(false);
			return;
		}

		m_entities.AddUnique(entity);
	}
	void ComponentManipulator::RegisterSystem(SharedPtr<System>&& system)
	{
		// TODO check for already registerd.
		
		system->SetComponentManipulator(this);

		m_systems.Add(system);
	}

	void ComponentManipulator::RegisterComponentContainer(SharedPtr<IComponentContainer>&& container)
	{
		const ContainerIdT containerId{ container->GetComponentId() };
		auto foundContainerIt{ m_componentContainers.find(containerId) };
		if(foundContainerIt != std::end(m_componentContainers))
		{
			//Found Container
			return;
		}
		else 
		{
			if(m_componentContainers.size() > MAX_COMPONENTS)
			{
				assert(false);
			}

			SharedPtr<IComponentContainer> newContainer{ std::move(container) };
			const uint8 componentSignature{ (uint8)m_componentContainers.size() };
			ComponentContainerInfo newContainerInfo{ newContainer, componentSignature };
						
			m_componentContainers[containerId] = newContainerInfo;					
		}
	}

	void ComponentManipulator::UpdateSystems(float deltaTime)
	{
		//Iterate all entities
		//Check if met requirements for certain system
		// T: Retreive the components for this entities.
		// Update system.

		for(auto& entity: m_entities)
		{
			auto signatureIt{ m_entitiesSignature.find(entity) };
			if(signatureIt == std::end(m_entitiesSignature))
			{				
				// Entity with no components
				continue;
			}

			const auto entitySignature{ signatureIt->second };
			for(auto& system: m_systems)
			{
				const auto systemSignature{ system->GetSignature() };
				if((systemSignature & entitySignature).any())
				{
					system->Update(deltaTime, entity);
				}
			}
		}

	}
}// ishak::Ecs

