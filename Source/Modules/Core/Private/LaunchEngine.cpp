#include "windows.h"
#include <iostream>

#include "GameLoop.h"
#include "Functionality/DynamicLibraryLoader.h"
#include "Module/ModuleManager.h"
#include "Functionality/Factory.h"
#include "World/GameInstance.h"

#include "World/World.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{			
	ishak::GameLoop gameLoop;

	ishak::DllLoader dll;
	dll.LoadEngineDlls();

	ishak::Factory factory;
	factory.RegisterBuilder<ishak::World>([](const ishak::Factory&)
	{
		return std::make_shared<ishak::World>();
	}, ishak::EClassMultiplicity::Singleton);
	
	
	ishak::ModuleManager::Get().InitModules(factory);

	auto f = factory.GetOrBuild<ishak::GameInstance>();

	f->Init();

	gameLoop.Init();
	gameLoop.Execute();	

		
	return 0;
}