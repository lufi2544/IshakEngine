#pragma once
#include "CoreMinimal.h"


namespace ishak::Ecs {

	/** This is for having the Maximum amount of components that I allow in the Engine.
	*  I think this may change in the future, but for now is okay.
	*/
	constexpr uint8 MAX_COMPONENTS = 32;

	/** Bitset for a System Signature for the necessary components that will Update certain systems.
	*   if we have the 0101 the components with the id = 1 and 3 those both classes will be updated.
	*/
	typedef std::bitset<MAX_COMPONENTS> Signature;

}// ishak::Ecs