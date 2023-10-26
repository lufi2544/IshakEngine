#include <iostream>
#include "Renderer.h"
#include "Window/Window.h"


namespace ishak {

	void Renderer::AddRenderingTarget(Window* window)
	{
		
		if(m_rendererWindowPair.first)
		{
			return;
		}

		// Create new Renderer and window associated.

		Uint32 extraRendererFlags = 0;
		if ((window->GetCreationContext().flags & WindowFlags::WINDOW_VSYNC) != 0)
		{
			extraRendererFlags |= SDL_RENDERER_ACCELERATED;
			extraRendererFlags |= SDL_RENDERER_PRESENTVSYNC;
		}

		SDL_Renderer* createdRenderer{ SDL_CreateRenderer(window->GetSDLWindow(), -1, extraRendererFlags) };

		if(!createdRenderer)
		{
			// TODO Add exception.
			return;
		}

		window->SetOnDestroyedDelegate([createdRenderer]()
		{
			// The renderer should always exist as long as the Window.
			if (createdRenderer)
			{
				SDL_DestroyRenderer(createdRenderer);
			}
		});


		m_rendererWindowPair = std::make_pair(window, createdRenderer);
	}

	void Renderer::Render()
	{
		// Render Windows		
		const Vector& windowColor{ m_rendererWindowPair.first->GetColor() };
		SDL_SetRenderDrawColor(m_rendererWindowPair.second, windowColor.r, windowColor.g, windowColor.b, windowColor.b);		
	}

	void Renderer::SubmitRendererCommand(const RendererCommand& command)
	{
		const bool bRenderColor{ command.texturePath.IsEmpty()};
		if(!m_rendererWindowPair.first)
		{
			return;
		}
		auto& renderer{ m_rendererWindowPair.second };
		if(bRenderColor)
		{
			SDL_SetRenderDrawColor(renderer, command.color.r, command.color.g, command.color.b, command.color.b);
			// Change this clear to somewhere else.
			SDL_RenderClear(m_rendererWindowPair.second);
		}else
		{
			SDL_Texture* cachedTexture{ m_rendererCache.GetTexture(command.texturePath, *this) };
			if(cachedTexture)
			{
				SDL_Rect destRect{ (int)command.position.x, (int)command.position.y, 32, 32 };				
				SDL_RenderCopy(renderer, cachedTexture, NULL, &destRect);
			}
		}
	}

	void Renderer::EndFrame()
	{
		SDL_RenderPresent(m_rendererWindowPair.second);
	}

	Renderer::RendererCache::~RendererCache()
	{
		for (auto& [textureName, texture] : textureCache)
		{
			SDL_DestroyTexture(texture);
		}
	}

	SDL_Texture* Renderer::RendererCache::GetTexture(const String& texturePath, Renderer& renderer)
	{
		if (!renderer.m_rendererWindowPair.first)
		{
			return nullptr;
		}

		auto foundIt{ textureCache.find(texturePath) };

		// found texture in cache
		if (foundIt != std::end(textureCache))
		{
			return foundIt->second;
		}
		else
		{

			SDL_Surface* surface = IMG_Load(texturePath.c_str());
			if(surface)
			{
				SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer.m_rendererWindowPair.second, surface);
				SDL_FreeSurface(surface);

				// not found, then create it in the cache
				textureCache.insert(std::make_pair(texturePath, texture));
			}
		}

		return nullptr;
	}

}// ishak
