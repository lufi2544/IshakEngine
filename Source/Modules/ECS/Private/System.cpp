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
		SetComponentRequirements();
	}


}// ishak::Ecs
