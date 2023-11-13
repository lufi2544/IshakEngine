#pragma once

#include "CoreMinimal.h"

#include "RendererConfig.h"
#include "Asset/Texture.h"



namespace ishak
{
	struct RENDERER_API TextureComponent
	{
		TextureComponent() = default;

		String* textureId;
		WeakPtr<Texture> texture;
	};

}//ishak