#pragma one
#include "SystemConfig.h"

namespace ishak { namespace Ecs{
	
	/** Bitset for a System Signature for the necessary components that will Update certain systems.
	*   if we have the 0101 the components with the id = 1 and 3 those both classes will be updated.
	*/
	typedef std::bitset<MAX_COMPONENTS> Signature;


}}// ishak::Ecs
