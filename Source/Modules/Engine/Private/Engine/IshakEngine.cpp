
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
#include "Ecs/Components/TransformComponent.h"
#include <Ecs/Components/TextureComponent.h>


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


		m_gameFramework.gameInstance->SetEcsContext(m_ecsContextContainer.get());
		RegisterEcsRenderingContainers(m_ecsContextContainer->GetEcsContext(Ecs::ContextID::CUSTOM)->GetComponentManipulator());

		// Create World
		ISHAK_LOG(Temp, "Creating World Context" )
		m_gameFramework.world = std::make_unique<World>(m_gameFramework.gameInstance);
		m_gameFramework.gameInstance->SetWorld(m_gameFramework.world.get());

		m_gameFramework.gameInstance->Init();
	}

	void IshakEngine::InitEngineCore()
	{		
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
			WindowFlags::WINDOW_CENTRALIZED | WindowFlags::WINDOW_VSYNC | WindowFlags::WINDOW_FULLSCREEN_MATCH_MONITOR
		};

		m_GameMainWindow = ishak::Window::MakeWindow(winCreationContext);
		Renderer::Get().AddRenderingTarget(m_GameMainWindow.get());

		auto& f{ Renderer::Get() };

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
		UniquePtr<GlobalEntityCreator> globalEntityCreator{ std::make_unique<GlobalEntityCreator>() };
		 
		// For now we have only one entity Manger, in the future I may add different EntityManagers, but for now is okay to have it this way.
		SharedPtr<Ecs::EntityManager> entityManager{ std::make_shared<Ecs::EntityManager>(std::move(globalEntityCreator)) };		
		m_ecsContextContainer = std::make_unique<Ecs::EcsContextContainer>(entityManager);

		CreateSharedComponentsContainers();
		InitCoreEngineEcs();
		InitRenderingEcs();
			
	}

	void IshakEngine::CreateSharedComponentsContainers()
	{
		// Add shared components containers along the different ecsContexts.
		SharedPtr<Ecs::IComponentContainer> transformContainer = std::make_shared<Ecs::ComponentContainer<TransformComponent>>();

		m_sharedComponentsContainers.Add(transformContainer);
	}

	void IshakEngine::RegisterSharedContainersInComponentManipulator(Ecs::ComponentManipulator* compManipulator)
	{
		for (auto container : m_sharedComponentsContainers)
		{
			compManipulator->RegisterComponentContainer(std::move(container));
		}
	}

	void IshakEngine::InitCoreEngineEcs()
	{			
		ISHAK_LOG(Temp, "Initializing ENGINE ECS..................")
		/* Add the Engine Init for CompManipulator and Systems here */
		UniquePtr<Ecs::ComponentManipulator> componentManipulator{ std::make_unique<Ecs::ComponentManipulator>() };
		Ecs::EcsContext* coreEcsContext = new Ecs::EcsContext(m_ecsContextContainer->entityManger, std::move(componentManipulator));
		
		RegisterEcsCoreContainers(coreEcsContext->GetComponentManipulator());
		RegisterEcsCoreSystems(coreEcsContext);

		m_ecsContextContainer->AddContext(coreEcsContext);
	}

	void IshakEngine::RegisterEcsCoreContainers(Ecs::ComponentManipulator* compMan)
	{
		//...

		RegisterSharedContainersInComponentManipulator(compMan);
	}

	void IshakEngine::RegisterEcsCoreSystems(Ecs::EcsContext* ecsContext)
	{
		//...
	}

	void IshakEngine::InitRenderingEcs()
	{
		ISHAK_LOG(Temp, "Initializing RENDERING ECS..................")
		UniquePtr<Ecs::ComponentManipulator> componentManipulator{ std::make_unique<Ecs::ComponentManipulator>() };
		Ecs::EcsContext* renderingEcsContext = new Ecs::EcsContext(m_ecsContextContainer->entityManger, std::move(componentManipulator));

		RegisterEcsRenderingContainers(renderingEcsContext->GetComponentManipulator());
		RegisterEcsRenderingSystems(renderingEcsContext);

		m_ecsContextContainer->AddContext(renderingEcsContext);
	}

	void IshakEngine::RegisterEcsRenderingContainers(Ecs::ComponentManipulator* compMan)
	{
		// Add the Rendering components 
		SharedPtr<Ecs::IComponentContainer> renderingComponentCon = std::make_shared<Ecs::ComponentContainer<RenderingComponent>>();					
		SharedPtr<Ecs::IComponentContainer> textureComponent = std::make_shared<Ecs::ComponentContainer<TextureComponent>>();
		compMan->RegisterComponentContainer(std::move(renderingComponentCon));		
		compMan->RegisterComponentContainer(std::move(textureComponent));

		RegisterSharedContainersInComponentManipulator(compMan);
	}

	void IshakEngine::RegisterEcsRenderingSystems(Ecs::EcsContext* ecsContext)
	{		
		// Add the Rendering Systems
		UniquePtr<RenderingSystem> renderingSys{ std::make_unique<RenderingSystem>() };

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
		UpdateCoreEngineEcs(deltaTime);
		UpdateCustomEcs(deltaTime);

		// TODO Remove this
		m_gameFramework.world->Update(deltaTime);
	}

	void IshakEngine::UpdateCoreEngineEcs(float dt)
	{
		m_ecsContextContainer->GetEcsContext(Ecs::ContextID::ENGINE)->UpdateContext(dt);
	}

	void IshakEngine::UpdateCustomEcs(float dt)
	{
		m_ecsContextContainer->GetEcsContext(Ecs::ContextID::CUSTOM)->UpdateContext(dt);
	}


	void IshakEngine::Render()
	{					
		// Here we would process the EcsContext related to the Redering
		// Then render the entities
				
		m_ecsContextContainer->GetEcsContext(Ecs::ContextID::RENDERER)->UpdateContext(0.00f);

		Renderer::Get().Render();
		Renderer::Get().EndFrame();
	}

	// Last chance to delete stuff.
	void IshakEngine::ShutDown()
	{			
		m_gameFramework.ShutDown();
		Renderer::Get().ShutDown();
	}

	void IshakEngine::GameFramework::ShutDown()
	{
		gameInstance->ShutDown();
	}

}// ishak