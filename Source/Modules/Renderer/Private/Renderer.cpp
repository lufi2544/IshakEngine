#include <iostream>
#include "Renderer.h"
#include "Window/Window.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "Log/Logger.h"


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
		PostSetRenderingTarget(window);
	}


	void Renderer::PreRender()
	{		
		SDL_RenderClear(m_rendererWindowPair.second);

	  //=========================================================================
	  // IMGUI
	  //=========================================================================
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();		
		//ImGui::ShowDemoWindow();
	  //=========================================================================
	}

	void Renderer::Render(const TArray<RendererCommand>& commands)
	{	
		PreRender();

		for (auto&& command : commands)
		{
			SubmitRendererCommand(command);
		}

		GLog->Draw();

		PostRender();
		
	}

	void Renderer::PostRender()
	{
	 //=========================================================================
	 // IMGUI
	 //=========================================================================
		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
	 //=========================================================================
	}

	void Renderer::PostSetRenderingTarget(Window* window)
	{
		//=========================================================================
		// IMGUI
		//=========================================================================	  
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls		

		SDL_Renderer* renderer{ m_rendererWindowPair.second };

		ImGui::StyleColorsDark();
		ImGui_ImplSDL2_InitForSDLRenderer(window->GetSDLWindow(), renderer);
		ImGui_ImplSDLRenderer2_Init(renderer);
		//=========================================================================

			
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
			SDL_SetRenderDrawColor(renderer, command.color.r, command.color.g, command.color.b, 0);			

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

				return texture;
			}
		}

		return nullptr;
	}

	void Renderer::ShutDown()
	{
	//=========================================================================
	// IMGUI
	//=========================================================================
		ImGui_ImplSDLRenderer2_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	//=========================================================================

	}

}// ishak
