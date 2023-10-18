
// Ishak
#include "EngineLoop.h"
#include "Engine/IshakEngine.h"


namespace ishak{

	IshakEngine* GEngine = nullptr;

	EngineLoop::EngineLoop() 
	{
		
	}

	EngineLoop::~EngineLoop() 
	{
		/*
		SDL_Quit();
		*/
		delete GEngine;
		GEngine = nullptr;
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

		return 0;
	}
} // ishak;