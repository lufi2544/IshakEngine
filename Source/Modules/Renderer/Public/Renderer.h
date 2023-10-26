#pragma once
#include "RendererConfig.h"
#include "CoreMinimal.h"
#include "SDL.h"
#include "SDL/SDL_image.h"
#include "RendererContext.h"

namespace ishak {

	class Window;
}


namespace ishak {

	/*======================================================
	* Global Renderer class for the engine.
	* The renderer will have a window associated for the rendering
	* pass.
	*======================================================*/
	class RENDERER_API Renderer
	{
		typedef SDL_Renderer* RendererT;
	public:
		Renderer() = default;
		void AddRenderingTarget(Window* window);
		
		void Render();
		void SubmitRendererCommand(const RendererCommand& command);
		void EndFrame();

		RendererT GetSDLRenderer()
		{
			return m_rendererWindowPair.second;
		}

			
	private:

		struct RENDERER_API RendererCache
		{
			~RendererCache();

			// TODO AssetManager for the textures
			inline SDL_Texture* GetTexture(const String& texturePath, Renderer& renderer);

			std::unordered_map<String, SDL_Texture*> textureCache;
		}m_rendererCache;

		std::pair<Window*, RendererT> m_rendererWindowPair;
		
	};

}// ishak
