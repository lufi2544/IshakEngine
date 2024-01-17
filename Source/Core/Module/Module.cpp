
#include "Core/Module/Module.h"
#include "Core/Module/ModuleManager.h"

namespace ishak {

	Module::Module()
	{
		ModuleManager& moduleManager{ ModuleManager::Get() };

		moduleManager.RegisterModule(this);
	}

	void Module::InitModule(Factory& factory)
	{
		// Do stuff...

		DoInitModule(factory);
	}

	void Module::ShutDownModule() 
	{
		// Shut down stuff...
		DoShutDownModule();
	}	

}// ishak