
// ISHAK
#include "Engine/Engine/IshakEngine.h"

#include "Core/Functionality/DynamicLibraryLoader.h"
#include "Core/Module/ModuleManager.h"
#include "Platform/Window/Window.h"
#include "Core/Log/Logger.h"

// GF
#include "Engine/GameFramework/World.h"
#include "Engine/GameFramework/GameInstance.h"

//IMGUI
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_sdl2.h"
#include "IMGUI/imgui_impl_sdlrenderer2.h"


// Renderer
#include "Renderer/Renderer.h"
#include "Renderer/Ecs/RenderingComponent.h"
#include "Renderer/Ecs/RenderingSystem.h"

// ECS
#include "Engine/Ecs/GlobalEntityCreator.h"
#include "Engine/Ecs/Components/LevelComponent.h"
#include "Engine/Ecs/Systems/GameFramework/LevelSystem.h"
#include "Core/Ecs/Components/TransformComponent.h"
#include "Core/Memory/MemoryManager.h"
#include "Renderer/Ecs/Components/TextureComponent.h"


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

		 //Create GameInstance
		m_gameFramework.gameInstance = factory.GetOrBuild<GameInstance>();

		if(m_gameFramework.gameInstance)
		{
			m_gameFramework.gameInstance->SetEcsContext(m_ecsContextContainer.get());
			RegisterEcsRenderingContainers(m_ecsContextContainer->GetEcsContext(Ecs::ContextID::CUSTOM)->GetComponentManipulator());

			// Create World
			ISHAK_LOG(Temp, "Creating World Context")
				m_gameFramework.world = std::make_unique<World>(m_gameFramework.gameInstance);
			m_gameFramework.gameInstance->SetWorld(m_gameFramework.world.get());

			m_gameFramework.gameInstance->Init();
		}
	}

	void IshakEngine::InitEngineCore()
	{		
		InitEngineMemory();

		// Init the Renderer
 		Renderer::Get().Init();

		// Init Main Window 
		ishak::WindowCreationContext winCreationContext = WindowCreationContext
		{
			"IshakEngine",
			0, 0,// where
			800, 600, // dimensions
			WindowFlags::WINDOW_CENTRALIZED | WindowFlags::WINDOW_VSYNC 
		};

	
		m_GameMainWindow = ishak::Window::MakeWindow(winCreationContext);
		Renderer::Get().AddRenderingTarget(m_GameMainWindow.get());
		
		InitEcs();
	}

	void IshakEngine::InitEngineMemory()
	{
		// Init Memory Allocator
		// We could retreive memory from the preallocated mememory here, so we would call malloc once and then get everything
		// from there.
		GAlloc = new FEngineMalloc();
		Memory::MemoryManager::Get().SetEngineAllocator(GAlloc);
	}

	void IshakEngine::HandleModules(Factory* factory)
	{					
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

		std::cout << "Engine Memory: " << ((float)Memory::MemoryManager::Get().GetMemoryUsage()) <<std::endl;

		// TODO Remove this
		m_gameFramework.world->Update(deltaTime);
	}

	void IshakEngine::UpdateCoreEngineEcs(float dt)
	{
		if(m_ecsContextContainer)
		{
			Ecs::EcsContext* engineEcsContext = { m_ecsContextContainer->GetEcsContext(Ecs::ContextID::ENGINE) };
			if(engineEcsContext)
			{
				engineEcsContext->UpdateContext(dt);
			}
		}
	}

	void IshakEngine::UpdateCustomEcs(float dt)
	{
		if(m_ecsContextContainer)
		{
			Ecs::EcsContext* ecsCustomContext = { m_ecsContextContainer->GetEcsContext(Ecs::ContextID::CUSTOM) };
			if(ecsCustomContext)
			{
				ecsCustomContext->UpdateContext(dt);
			}
		}
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
		delete GAlloc;
		// TODO ISHMEMORY maybe just let the alloator reside in the MemoryManager.

	}

	void IshakEngine::GameFramework::ShutDown()
	{
		if(gameInstance)
		{
			gameInstance->ShutDown();
		}
	}

}// ishak
