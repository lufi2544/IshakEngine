#include "windows.h"
#include <iostream>

#include "Array.h"
#include "GameLoop.h"
#include "DynamicLibraryLoader.h"
#include "ModuleManager.h"
#include "Factory.h"

#include "World.h"

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

	gameLoop.Init();
	gameLoop.Execute();	

		
	return 0;
}