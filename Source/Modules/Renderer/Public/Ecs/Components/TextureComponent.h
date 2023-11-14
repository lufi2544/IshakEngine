#pragma once

#include "CoreMinimal.h"

#include "RendererConfig.h"
#include "Asset/Texture.h"



namespace ishak
{
	struct RENDERER_API TextureComponent
	{
		TextureComponent() = default;

		String* texturePath{ nullptr };
		WeakPtr<Texture> texture;
		Vector2 renderingCoordinates{ -1, -1 };
		int customWidht{ 0 };
		int customHeight{ 0 };
	};

}//ishak