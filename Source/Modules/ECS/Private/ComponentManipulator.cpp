#include "ComponentManipulator.h"

namespace ishak::Ecs
{
	void ComponentManipulator::RegisterSystem(const ISystem& system)
	{
		// TODO check for already registerd.

		m_systems.Add(system);
	}

	void ComponentManipulator::RegisterComponentContainer(const SharedPtr<IComponentContainer>& container)
	{
		m_componentContainers.insert(std::make_pair(container->GetId(), container));
	}

	void ComponentManipulator::UpdateSystems()
	{
		//Iterate all entities
		//Check if met requirements for certain system
		// T: Retreive the components for this entities.
		// Update system.
	}
}// ishak::Ecs

