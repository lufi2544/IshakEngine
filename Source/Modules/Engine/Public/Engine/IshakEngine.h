#pragma once
#include "CoreMinimal.h"
#include "EngineConfig.h"
#include "GameFramework/World.h"
#include "Functionality/Factory.h"

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

	private:
		void HandleModules(Factory* factory);
		void Run();
		void ShutDown();

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
		} m_gameFramework;

	};
		
	extern CORE_API IshakEngine* GEngine;
}// ishak