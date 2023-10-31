#include "System.h"
#include "Component.h"

namespace ishak::Ecs {

	System::System()
	{

	}

	TArray<EntityId> System::GetSystemEntities() const
	{
		return TArray<EntityId>();
	}

	Signature& System::GetComponentSignature()
	{
		return m_signature;
	}

	void System::AddEntityToSystem(EntityId entityId)
	{
		m_entities.Add(entityId);
	}

	void System::RemoveEntityFromSystem(EntityId entityId)
	{
		m_entities.Remove(entityId);
	}

	template<typename ComponentT>
	void System::RequireComponent()
	{
		const ComponentId id{ ComponentT::GetId() };
		m_signature.set(id);
	}

}// ishak::Ecs
