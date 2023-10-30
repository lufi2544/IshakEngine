
// Ishak
#include "EngineLoop.h"
#include "Engine/IshakEngine.h"

#include "Module/ModuleManager.h"

#include "CoreMinimal.h"


namespace ishak{

	CORE_API IshakEngine* GEngine = nullptr;

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

			// Since Last Engine Tick
			const float tickDeltaTime{ duration_cast<duration<float>>(currentTime - previusTime).count() };
			
			previusTime = currentTime;
			accumulatedTime += tickDeltaTime;

			//Since 
			while(accumulatedTime >= FIXED_DELTA)
			{
				GEngine->ProcessInput();
				GEngine->Tick(FIXED_DELTA);

				accumulatedTime -= FIXED_DELTA;
			}

			// If arrived here fast, then we sleep what is needed to fini
			if(accumulatedTime < FIXED_DELTA)
			{
				float toSleep{ FIXED_DELTA - accumulatedTime };
				Sleep(toSleep);
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