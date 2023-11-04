#include "ComponentManipulator.h"

namespace ishak::Ecs
{
	void ComponentManipulator::RegisterSystem(const SharedPtr<ISystem>& system)
	{
		// TODO check for already registerd.

		m_systems.Add(system);
	}

	void ComponentManipulator::RegisterComponentContainer(const SharedPtr<IComponentContainer>& container)
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
			//New Container
			m_componentContainers[containerId] = container;
		}
	}

	void ComponentManipulator::UpdateSystems(float deltaTime)
	{
		//Iterate all entities
		//Check if met requirements for certain system
		// T: Retreive the components for this entities.
		// Update system.

		

	}
}// ishak::Ecs

