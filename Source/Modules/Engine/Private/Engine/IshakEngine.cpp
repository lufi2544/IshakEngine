
// ISHAK
#include "Engine/IshakEngine.h"

#include "Functionality/DynamicLibraryLoader.h"
#include "Module/ModuleManager.h"
#include "Window/Window.h"
#include "Log/Logger.h"

// GF
#include "GameFramework/World.h"
#include "GameFramework/GameInstance.h"

//IMGUI
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"


// Renderer
#include "Renderer.h"
#include "Ecs/RenderingComponent.h"
#include "Ecs/RenderingSystem.h"

// ECS
#include "Ecs/GlobalEntityCreator.h"
#include "Ecs/Components/LevelComponent.h"
#include "Ecs/Systems/GameFramework/LevelSystem.h"


namespace ishak {	
	

	IshakEngine::IshakEngine()
	{		
	}

	void IshakEngine::Init()
	{		

		ISHAK_LOG(Temp, "Engine Init.")
		ISHAK_LOG(Temp, "Loading Modules Dlls....")
		Factory factory;
		HandleModules(&factory);	

		InitEngineCore();	

		// Create GameInstance
		m_gameFramework.gameInstance = factory.GetOrBuild<GameInstance>();


		m_gameFramework.gameInstance->SetEcsContext(&m_ecsContextContainer);

		// Create World
		ISHAK_LOG(Temp, "Creating World Context" )
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

	void IshakEngine::InitEngineCore()
	{
		m_renderer = std::make_unique<Renderer>();

		InitEcs();
	}

	void IshakEngine::HandleModules(Factory* factory)
	{					
		// Load the Modules into memory and the init them.
		ishak::ModuleManager::Get().LoadModules();
		ishak::ModuleManager::Get().InitModules(factory);		
	}

	void IshakEngine::InitEcs()
	{
		ISHAK_LOG(Temp, "Initializing ENGINE ECS..................")


		// Prepare the ContextContainer
		m_ecsContextContainer.Reserve(Ecs::ECSContextID::NUM);

		UniquePtr<GlobalEntityCreator> globalEntityCreator{ std::make_unique<GlobalEntityCreator>() };
		 
		// For now we have only one entity Manger, in the future I may add different EntityManagers, but for now is okay to have it this way.
		SharedPtr<Ecs::EntityManager> entityManager{ std::make_unique<Ecs::EntityManager>(std::move(globalEntityCreator)) };

		InitCoreEngineEcs(entityManager);
		InitRenderingEcs(entityManager);
			
	}

	void IshakEngine::InitCoreEngineEcs(SharedPtr<Ecs::EntityManager> entityManager)
	{			
		/* Add the Engine Init for CompManipulator and Systems here */
		UniquePtr<Ecs::ComponentManipulator> componentManipulator{ std::make_unique<Ecs::ComponentManipulator>() };
		SharedPtr<Ecs::EcsContext> coreEcsContext = std::make_shared<Ecs::EcsContext>(std::move(entityManager), std::move(componentManipulator));
		
		RegisterEcsCoreContainers(coreEcsContext->GetComponentManipulator());
		RegisterEcsCoreingSystems(coreEcsContext.get());

		m_ecsContextContainer.Add(std::move(coreEcsContext));
	}

	void IshakEngine::RegisterEcsCoreContainers(Ecs::ComponentManipulator* compMan)
	{
		//...
	}

	void IshakEngine::RegisterEcsCoreingSystems(Ecs::EcsContext* ecsContext)
	{
		//...
	}

	void IshakEngine::InitRenderingEcs(SharedPtr<Ecs::EntityManager> entityManager)
	{
		UniquePtr<Ecs::ComponentManipulator> componentManipulator{ std::make_unique<Ecs::ComponentManipulator>() };
		SharedPtr<Ecs::EcsContext> renderingEcsContext = std::make_shared<Ecs::EcsContext>(std::move(entityManager), std::move(componentManipulator));

		RegisterEcsRenderingContainers(renderingEcsContext->GetComponentManipulator());
		RegisterEcsRenderingSystems(renderingEcsContext.get());

		m_ecsContextContainer.Add(std::move(renderingEcsContext));
	}

	void IshakEngine::RegisterEcsRenderingContainers(Ecs::ComponentManipulator* compMan)
	{
		// Add the Rendering components 
		SharedPtr<Ecs::IComponentContainer> renderingComponentCon = std::make_shared<Ecs::ComponentContainer<RenderingComponent>>();					
		compMan->RegisterComponentContainer(std::move(renderingComponentCon));		
	}

	void IshakEngine::RegisterEcsRenderingSystems(Ecs::EcsContext* ecsContext)
	{		
		// Add the Rendering Systems
		UniquePtr<RenderingSystem> renderingSys{ std::make_unique<RenderingSystem>(m_renderer.get()) };		

		//**NOTE: Registration is the Update order.		
		ecsContext->RegisterSystem(std::move(renderingSys));
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
		ISHAK_LOG(Warning, "TICKING")
		UpdateCoreEngineEcs(deltaTime);

		m_gameFramework.world->Update(deltaTime);
	}

	void IshakEngine::UpdateCoreEngineEcs(float dt)
	{
		m_ecsContextContainer[Ecs::ECSContextID::ENGINE]->UpdateContext(dt);
	}


	void IshakEngine::Render()
	{			
		ISHAK_LOG(Warning, "RENDERING")

		// Here we would process the EcsContext related to the Redering
		// Then render the entities
				
		m_ecsContextContainer[Ecs::ECSContextID::RENDERER]->UpdateContext(0.00f);

		m_renderer->Render();					
		m_renderer->EndFrame();
	}

	// Last chance to delete stuff.
	void IshakEngine::ShutDown()
	{			
		m_gameFramework.ShutDown();
		m_renderer->ShutDown();
	}

	void IshakEngine::GameFramework::ShutDown()
	{
		gameInstance->ShutDown();
	}

}// ishak