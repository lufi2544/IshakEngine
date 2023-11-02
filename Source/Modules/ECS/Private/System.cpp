#include "System.h"
#include "Component.h"

namespace ishak::Ecs {

	System::System()
	{

	}

	Signature& System::GetComponentSignature()
	{
		return m_signature;
	}

	template<typename ComponentT>
	void System::RequireComponent()
	{
		const ComponentId id{ ComponentT::GetId() };
		m_signature.set(id);
	}

}// ishak::Ecs
