#include <iostream>
#include "Renderer.h"
#include "Window/Window.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "Log/Logger.h"


namespace ishak {

	Renderer* Renderer::singleton = nullptr;

	Renderer& Renderer::Get()
	{		
		if(!singleton)
		{
			singleton = new Renderer();
		}

		return *singleton;
	}

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

		for (auto& [entity, renderingCommandPair] : m_frameCommandsQueue)
		{
			SubmitRendererCommand(renderingCommandPair.second);			
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
		// TODO Create a data structure for the Z Order;
		if(commandedEntities.Contains(command.entityId))
		{
			// entity already registered
			return;
		}
		
		m_frameCommandsQueue.insert(std::make_pair(command.zOrder, std::make_pair(command.entityId, command)));
		commandedEntities.Add(command.entityId);
	}	

	void Renderer::ClearFrameRendererCommands()
	{
		m_frameCommandsQueue.clear();
		commandedEntities.Clear();
	}

	void Renderer::SubmitRendererCommand(const RendererCommand& command)
	{
		if(command.entityId == Ecs::kNullId)
		{
			return;
		}
											
		if (command.texture)
		{	
			bool bShouldRenderFromCoordinates{ false };
			SDL_Rect srcRectangle;
			// Check wether we should render the texture from a certain coordinate.
			if(command.renderingCoordinates.x != -1 || command.renderingCoordinates.y != -1)
			{				
				srcRectangle = SDL_Rect{ (int)command.renderingCoordinates.x, (int)command.renderingCoordinates.y, command.scaledW, command.scaledH };
				bShouldRenderFromCoordinates = true;
			}

			SDL_Rect destRect{ (int)command.position.x, (int)command.position.y, command.scaledW, command.scaledH };
			SDL_RenderCopyEx(
				GetSDLRenderer(),
				command.texture->GetSDLTexture(),
				bShouldRenderFromCoordinates ? &srcRectangle :  NULL,
				&destRect,
				command.rotation,
				NULL, 
				SDL_FLIP_NONE);
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
