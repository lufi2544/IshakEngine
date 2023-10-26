
// ISHAK
#include "Engine/IshakEngine.h"

#include "Functionality/DynamicLibraryLoader.h"
#include "Module/ModuleManager.h"
#include "Window/Window.h"
#include "GameFramework/World.h"
#include "GameFramework/GameInstance.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "Renderer.h"
#include "Log/Logger.h"

namespace ishak {	
		

	Logger* GLogger = nullptr;

	IshakEngine::IshakEngine()
	{		
	}

	void IshakEngine::Init()
	{		
		Factory factory;
		HandleModules(&factory);	

		m_renderer = std::make_unique<Renderer>();

		// Create GameInstance
		m_gameFramework.gameInstance = factory.GetOrBuild<GameInstance>();


		// Create World
		m_gameFramework.world = std::make_unique<World>(m_gameFramework.gameInstance);
		m_gameFramework.gameInstance->SetWorld(m_gameFramework.world.get());

		m_gameFramework.gameInstance->Init();


		// Init Main Window and Rendering stuff.
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			std::cerr << "Error initializing SDL." << std::endl;
			return;
		}

		ishak::WindowCreationContext winCreationContext = WindowCreationContext
		{
			"IshakEngine",
			0, 0,// where
			800, 600, // dimensions
			WindowFlags::WINDOW_CENTRALIZED  |  WindowFlags::WINDOW_VSYNC | WindowFlags::WINDOW_FULLSCREEN_MATCH_MONITOR
		};

		m_GameMainWindow = ishak::Window::MakeWindow(winCreationContext);		
		m_renderer->AddRenderingTarget(m_GameMainWindow.get());			
	}

	void IshakEngine::HandleModules(Factory* factory)
	{					
		// Load the Modules into memory and the init them.
		ishak::ModuleManager::Get().LoadModules();
		ishak::ModuleManager::Get().InitModules(factory);		
	}

	void IshakEngine::ProcessInput()
	{
		SDL_Event sdlEvent;
		while (SDL_PollEvent(&sdlEvent))
		{
			//TODO I think this should be on a different place.
			ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
			switch (sdlEvent.type)
			{
			case SDL_QUIT:
				bWantsToExit = true;

				break;
			case SDL_KEYDOWN:
				if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
				{
					bWantsToExit = true;
				}
				break;

			default:
				break;
			}
		}
	}

	void IshakEngine::Tick(float deltaTime)
	{				
		m_gameFramework.world->Update(deltaTime);
	}

	void IshakEngine::Render()
	{					
		// Render all entities.
		TArray<RendererCommand> renderingCommands;

		// Window color Render first
		RendererCommand command;
		command.color = m_GameMainWindow->GetColor();		
		renderingCommands.Add(command);

		// Then render the entities
		m_gameFramework.world->DoInAllEntities([&renderingCommands](SharedPtr<Entity>& entity) 
		{
			if(entity->GetTexture().IsEmpty())
			{
				return;
			}							

			RendererCommand command;
			command.texturePath = entity->GetTexture();
			command.position = entity->GetPosition();

			renderingCommands.Add(command);				
		});

		m_renderer->Render(renderingCommands);					
		m_renderer->EndFrame();
	}

	// Last chance to delete stuff.
	void IshakEngine::ShutDown()
	{			
		m_renderer->ShutDown();
	}
}// ishak