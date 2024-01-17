#include "Ecs/System.h"
#include "Ecs/ComponentManipulator.h"

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
