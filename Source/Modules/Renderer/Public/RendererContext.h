#pragma once

#include "RendererConfig.h"
#include "CoreMinimal.h"


namespace ishak{

	struct RENDERER_API RendererCommand
	{
		std::string texturePath;
		Vector color;
		Vector2 position;
	};

}// ishak