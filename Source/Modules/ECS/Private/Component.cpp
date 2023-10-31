#include "Component.h"

namespace ishak::Ecs {


	template<typename T>
	Component<T>::Component()
	{

	}

	template<typename T>
	uint16 Component<T>::GetId()
	{
		return kIdGenerator++;
	}


}// ishak::Ecs