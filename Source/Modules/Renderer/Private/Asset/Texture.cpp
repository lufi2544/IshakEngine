#include "Asset/Texture.h"

namespace ishak
{
	Texture::Texture(const String& id, SDL_Texture* sdltexture, int w, int h)
		: Asset(id)
		, m_sdlTexture{ sdltexture }
		, width{ w }
		, height{ h }
	{

	}
	Texture::~Texture()
	{
		SDL_DestroyTexture(m_sdlTexture);
		m_sdlTexture = nullptr;
	}
}//ishak