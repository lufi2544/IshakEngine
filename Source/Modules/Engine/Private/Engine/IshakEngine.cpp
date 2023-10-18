
// ISHAK
#include "Engine/IshakEngine.h"

#include "Functionality/DynamicLibraryLoader.h"
#include "Module/ModuleManager.h"
#include "Window/Window.h"
#include "GameFramework/World.h"
#include "GameFramework/GameInstance.h"

namespace ishak {	
		
	IshakEngine::IshakEngine()
	{		
	}

	void IshakEngine::Init()
	{		
		Factory factory;
		HandleModules(&factory);	

		// Create GameInstance
		m_gameFramework.gameInstance = factory.GetOrBuild<GameInstance>();

		// Create World
		m_gameFramework.world = std::make_unique<World>(m_gameFramework.gameInstance.get());
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
			WindowFlags::WINDOW_CENTRALIZED  |  WindowFlags::WINDOW_VSYNC
		};

		m_GameMainWindow = ishak::Window::MakeWindow(winCreationContext);
		
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
		if (m_GameMainWindow)
		{
			m_GameMainWindow->Render();
		}
	}

	void IshakEngine::Run()
	{
		
	}

	// Last chance to delete stuff.
	void IshakEngine::ShutDown()
	{
		// Unload the Modules from memory.
		ishak::ModuleManager::Get().UnloadModules();
	}
}// ishak