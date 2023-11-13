#include "Asset/Texture.h"

namespace ishak
{
	Texture::Texture(const String& id, SDL_Texture* sdltexture)
		: Asset(id)
		, m_sdlTexture{ sdltexture }
	{

	}
	Texture::~Texture()
	{
		SDL_DestroyTexture(m_sdlTexture);
		m_sdlTexture = nullptr;
	}
}//ishak