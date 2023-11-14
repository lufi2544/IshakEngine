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
		Texture(const String& id, SDL_Texture* sdltexture, int w, int h);
		inline SDL_Texture* GetSDLTexture() { return m_sdlTexture; }		

		int GetWidth() const
		{
			return width;
		}

		int GetHeight() const
		{
			return height;
		}


		~Texture();

	private:
		SDL_Texture* m_sdlTexture{ nullptr };
		int width{ 0 };
		int height{ 0 };
	};


}//ishak