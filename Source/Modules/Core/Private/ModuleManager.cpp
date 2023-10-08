
#include "ModuleManager.h"

namespace ishak {
	
	ModuleManager* ModuleManager::m_singleton = nullptr;

	ModuleManager& ModuleManager::Get()
	{
		if (m_singleton == nullptr)
		{
			m_singleton = new ModuleManager();
		}

		return *m_singleton;
	}

	void ModuleManager::RegisterModule(Module* moduleToAdd)
	{
		if(!moduleToAdd)
		{
			return;
		}		
		m_modules.Add(moduleToAdd);
	}

	void ModuleManager::InitModules(Factory& factory)
	{
		for (auto&& module : m_modules)
		{
			if(!module)
			{
				continue;
			}

			module->InitModule(factory);
		}
	}

}// ishak