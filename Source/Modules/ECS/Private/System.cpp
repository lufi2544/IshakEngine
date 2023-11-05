#include "System.h"
#include "Component.h"

#include "ComponentManipulator.h"

namespace ishak::Ecs {

	System::System()		
	{

	}

	Signature& System::GetSignature()
	{
		return m_signature;
	}

	void System::SetComponentManipulator(ComponentManipulator* compManipulator)
	{
		m_compManipulator = compManipulator;
		SetRequirements();
	}

	template<typename ComponentT>
	void System::RequireComponent()
	{
		if(!m_compManipulator)
		{
			assert(false);
			return;
		}
		m_signature.set(m_compManipulator->GetComponentContainerSignatureId<ComponentT>());
	}

	HealthSystem:: HealthSystem()
	{

	}

	void HealthSystem::Update(float dt, EntityId entity)
	{

	}
	void HealthSystem::SetRequirements()
	{
		RequireComponent<TransformComponent>();
	}

}// ishak::Ecs
