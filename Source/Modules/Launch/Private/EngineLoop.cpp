
// Ishak
#include "EngineLoop.h"
#include "Engine/IshakEngine.h"

#include "Module/ModuleManager.h"


namespace ishak{

	IshakEngine* GEngine = nullptr;

	EngineLoop::EngineLoop() 
	{
		
	}

	EngineLoop::~EngineLoop() 
	{

	}

	void EngineLoop::Init() 
	{
		// For now just create default Engine, I will include the Editor here.
		GEngine = new IshakEngine();		
		GEngine->Init();
	}

	int EngineLoop::TickEngine()
	{
		while (!GEngine->bWantsToExit) 
		{
			// TODO Add Real Delta Time
			GEngine->ProcessInput();
			GEngine->Tick(0.016f);
			GEngine->Render();
		}

		FinishProgram();

		return 0;
	}

	void EngineLoop::FinishProgram()
	{
		//// Shut Down Engine ////
		GEngine->ShutDown();

		if (GEngine)
		{
			delete GEngine;
			GEngine = nullptr;
		}


		//// Unload the Modules from memory ////
		ishak::ModuleManager::Get().UnloadModules();
	}
} // ishak;