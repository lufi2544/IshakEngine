#include "Asset/Texture.h"
#include "Renderer.h"
#include "Asset/AssetManager.h"

#include "Log/Logger.h"

namespace ishak
{
	Texture::Texture(const String& id, SDL_Texture* sdltexture, int w, int h)
		: Asset(id)
		, m_sdlTexture{ sdltexture }
		, m_width{ w }
		, m_height{ h }
	{
		
	}

	Texture::Texture(const String& path)
		: Asset(path)
	{
	}

	SharedPtr<Asset> Texture::Load()
	{

		SDL_Surface* image = IMG_Load(m_path.c_str());
		if (image)
		{
			m_sdlTexture = SDL_CreateTextureFromSurface(Renderer::Get().GetSDLRenderer(), image);
			m_width = image->w;
			m_height = image->h;
			//TODO Refactor, make an specialization of the fuction in the Asset Manger, so the asset manater created the memory for this.			
			SDL_FreeSurface(image);

			SharedPtr<Asset> returnThis{ shared_from_this() };
			
			return returnThis;
		}

		// TODO String, create Format
		std::string error = "Eror Loading Texture: " + std::string(m_path.c_str());		
		ISHAK_LOG(Error, error.c_str());

		return nullptr;
	}

	Texture::~Texture()
	{
		if(m_sdlTexture)
		{
			SDL_DestroyTexture(m_sdlTexture);
			m_sdlTexture = nullptr;
		}
	}
}//ishak
