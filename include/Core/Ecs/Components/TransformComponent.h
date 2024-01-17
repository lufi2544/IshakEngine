#pragma once
#include "Core/CoreConfig.h"
#include "Core/CoreMinimal.h"

namespace ishak
{
	struct  CORE_API TransformComponent
	{
		Vector2 position{ 0, 0 };
		float rotation{ 0.0f };		
		float scale{ 1.0f };
	};
	

}//ishak