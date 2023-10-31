#pragma once
#include "CoreMinimal.h"

namespace ishak::Ecs
{

	typedef uint8 ComponentId;

	template<typename T>
	class Component 
	{
	public:
		Component();

		static ComponentId GetId();

	protected:
		static ComponentId kIdGenerator;
	};


}// ishak::Ecs