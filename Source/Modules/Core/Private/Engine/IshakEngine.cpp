
// ISHAK
#include "Engine/IshakEngine.h"

#include "Functionality/DynamicLibraryLoader.h"
#include "Functionality/Factory.h"
#include "World/World.h"
#include "Module/ModuleManager.h"
#include "GameLoop.h"

namespace ishak {	

	IshakEngine* GEngine = nullptr;
	
	IshakEngine::IshakEngine()
	{		
	}

	int IshakEngine::Execute()
	{
		GEngine = new IshakEngine();
		GEngine->Init();
		GEngine->Run();
		GEngine->ShutDown();

		// Once finished the Engie execution, we free the Engine memory.
		delete GEngine;
		GEngine = nullptr;

		return 0;
	}

	void IshakEngine::Init()
	{	
		HandleModules();		
	}

	void IshakEngine::HandleModules()
	{
		// Load Modules Dlls into memory
		DllLoader moduleDllLoader;
		moduleDllLoader.LoadEngineDlls();


		// Create Factory for global classes creation.
		ishak::Factory factory;
		factory.RegisterBuilder<ishak::World>([](const ishak::Factory&)
			{
				return std::make_shared<ishak::World>();
			}, ishak::EClassMultiplicity::Singleton);


		// Call Init the Modules
		ishak::ModuleManager::Get().InitModules(&factory);
	}

	void IshakEngine::Run()
	{
		// GameLoop
		ishak::GameLoop gameLoop;
		gameLoop.Init();
		gameLoop.Execute();
	}

	void IshakEngine::ShutDown()
	{

	}
}// ishak