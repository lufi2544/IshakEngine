
// Ishak
#include "EngineLoop.h"
#include "Engine/IshakEngine.h"

#include "Module/ModuleManager.h"

#include "CoreMinimal.h"


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
		using namespace std::chrono;

		high_resolution_clock::time_point previusTime{ high_resolution_clock::now() };
		float accumulatedTime{ 0.0f };

		while (!GEngine->bWantsToExit) 
		{
			high_resolution_clock::time_point currentTime{ high_resolution_clock::now() };

			// Since Engine Start
			duration<float> timeSpan{ duration_cast<duration<float>>(currentTime - previusTime) };

			const float framesDeltaTime{ timeSpan.count() };

			previusTime = currentTime;
			accumulatedTime += framesDeltaTime;


			while(accumulatedTime >= FIXED_DELTA)
			{
				GEngine->ProcessInput();
				GEngine->Tick(FIXED_DELTA);

				accumulatedTime -= FIXED_DELTA;
			}
			
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
		ModuleManager::Get().UnloadModules();
	}
} // ishak;