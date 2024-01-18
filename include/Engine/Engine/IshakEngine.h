#pragma once
#include "Core/CoreMinimal.h"
#include "Engine/EngineConfig.h"
#include "Engine/GameFramework/World.h"
#include "Core/Functionality/Factory.h"
#include "Renderer/Renderer.h"

// ECS
#include "Ecs/Ecs.h"

namespace ishak {

	class Window;
	class GameInstance;	
}

namespace ishak {

	/* 
	*  This is the main Engine class.This will control the flow of the
	* initialization for modules, global managers, etc. When adding the Editor
	* I might interface this? I have to figure it out.
	*/
	class ENGINE_API IshakEngine
	{				
	public:
		IshakEngine();
		void Init();	
		void ProcessInput();
		void Tick(float deltaTime);
		void Render();
		void ShutDown();

	private:
		void HandleModules(Factory* factory);
		void InitEngineCore();

		void InitEngineMemory();
		void InitEcs();
		void CreateSharedComponentsContainers();
		void RegisterSharedContainersInComponentManipulator(Ecs::ComponentManipulator* compManipulator);
		void InitCoreEngineEcs();
		void RegisterEcsCoreContainers(Ecs::ComponentManipulator* compMan);
		void RegisterEcsCoreSystems(Ecs::EcsContext* ecsContext);

		void InitRenderingEcs();
		void RegisterEcsRenderingContainers(Ecs::ComponentManipulator* compMan);
		void RegisterEcsRenderingSystems(Ecs::EcsContext* ecsContext);

		void UpdateCoreEngineEcs(float dt);
		void UpdateCustomEcs(float dt);


	public:
		bool bWantsToExit{ false };

		// TODO Window Manager.
		SharedPtr<Window> m_GameMainWindow;		
				
		/** Wrapped up all the game framework stuff here for now. */
		struct GameFramework
		{

			// World is gonna be unique as not server travel or net will be added for now.
			// When added Editor, different world for the editor view windows will be added
			UniquePtr<World> world;

			// TODO Factory, change this to unique ptr from the factory.
			SharedPtr<GameInstance> gameInstance;
			void ShutDown();
		}m_gameFramework;

		
		/** EcsContext Array for the Engine.
		* We have to create different ecs contexts for the different parts that need to be updated.
		* The Rendering and the Core Engine Logic will have different Contexts
		*/
		UniquePtr<Ecs::EcsContextContainer> m_ecsContextContainer;	
		TArray<SharedPtr<Ecs::IComponentContainer>> m_sharedComponentsContainers;

		// TODO Refactor this!!
	};
		
	extern CORE_API IshakEngine* GEngine;
}// ishak