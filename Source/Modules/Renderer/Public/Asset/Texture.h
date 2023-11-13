#pragma once
#include "CoreMinimal.h"
#include "Asset/Asset.h"
#include "RendererConfig.h"

#include "SDL.h"
#include "SDL/SDL_image.h"
namespace ishak
{
	class RENDERER_API Texture : public Asset
	{
	public:
		Texture(const String& id, SDL_Texture* sdltexture);
		inline SDL_Texture* GetSDLTexture() { return m_sdlTexture; }		

		~Texture();

	private:
		SDL_Texture* m_sdlTexture{ nullptr };
	};


}//ishak