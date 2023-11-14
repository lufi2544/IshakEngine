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

		SDL_SetRenderDrawColor(m_rendererWindowPair.second, 21, 21, 21, 255);
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

	void Renderer::Render()
	{	
		PreRender();

		if(m_frameCommandsQueue.size() == 0)
		{
			ISHAK_LOG(Error, "RendererCommandsEmpty")
		}

		for (auto& [entity, renderingCommand] : m_frameCommandsQueue)
		{
			SubmitRendererCommand(renderingCommand);			
		}

		ClearFrameRendererCommands();

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

	void Renderer::QueueRenderCommand(const RendererCommand& command)
	{
		auto foundIt{ m_frameCommandsQueue.find(command.entityId) };
		if(foundIt != std::end(m_frameCommandsQueue))
		{
			// if found, we submit the command
			return;			
		}

		m_frameCommandsQueue.insert(std::make_pair(command.entityId, command));
	}	

	void Renderer::ClearFrameRendererCommands()
	{
		m_frameCommandsQueue.clear();
	}

	void Renderer::SubmitRendererCommand(const RendererCommand& command)
	{
		if(command.entityId == Ecs::kNullId)
		{
			return;
		}
											
		if (command.texture)
		{
			SDL_Rect destRect{ (int)command.position.x, (int)command.position.y, command.scaledW, command.scaledH };
			SDL_RenderCopyEx(GetSDLRenderer(), command.texture->GetSDLTexture(), NULL, &destRect, command.rotation, NULL, SDL_FLIP_NONE);
		}
		
	}

	void Renderer::EndFrame()
	{	
		SDL_RenderPresent(m_rendererWindowPair.second);
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
