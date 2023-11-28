#include "ComponentManipulator.h"

#include "chrono"

#define MODE 1

namespace ishak::Ecs
{
	Signature ComponentManipulator::GetEntitySignature(EntityId entity, bool& bFound)
	{
		auto foundIt{ m_entitiesSignaturesMap.find(entity) };
		if(foundIt == std::end(m_entitiesSignaturesMap))
		{
			bFound = false;
			return { };
		}

		bFound = true;
		return m_entitiesSignaturesMap[entity];
	}
	void ComponentManipulator::UnregisterEntitySignature(EntityId entity)
	{
		auto foundIt{ m_entitiesSignaturesMap.find(entity) };

		if(foundIt == std::end(m_entitiesSignaturesMap))
		{
			return;
		}

		m_entitiesSignaturesMap.erase(foundIt);
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

	void ComponentManipulator::UpdateComponentsStates()
	{
		if(m_commandQueue.IsEmpty())
		{
			return;
		}

		for(ComponentQueueCommandT& command : m_commandQueue)
		{
			std::invoke(command);
		}

		m_commandQueue.Clear();
	}
}// ishak::Ecs

