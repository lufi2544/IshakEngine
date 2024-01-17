#pragma once
#include "Core/CoreMinimal.h"
#include "Core/Asset/Asset.h"
#include "Renderer/RendererConfig.h"

#include "ThirdParty/SDL/SDL.h"
namespace ishak
{
	class RENDERER_API Texture : public Asset
	{
	public:
		Texture(const String& id, SDL_Texture* sdltexture, int w, int h);
		Texture(const String& path);
		inline SDL_Texture* GetSDLTexture() { return m_sdlTexture; }		

		SharedPtr<Asset> Load() override;

		int GetWidth() const
		{
			return m_width;
		}

		int GetHeight() const
		{
			return m_height;
		}


		~Texture();

	private:
		SDL_Texture* m_sdlTexture{ nullptr };
		int m_width{ 0 };
		int m_height{ 0 };
	};


}//ishak
