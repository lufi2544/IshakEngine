#pragma once

#include "RendererConfig.h"
#include "CoreMinimal.h"


namespace ishak{

	struct RENDERER_API RendererCommand
	{
		String texturePath;
		Vector4 color;
		Vector2 position;
	};

}// ishak