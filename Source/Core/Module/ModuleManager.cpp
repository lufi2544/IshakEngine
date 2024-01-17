
#include "Core/Module/ModuleManager.h"

namespace ishak {
	
	ModuleManager* ModuleManager::m_singleton = nullptr;

	ModuleManager::~ModuleManager()
	{
		if(m_singleton)
		{
			delete m_singleton;
		}

		m_singleton = nullptr;
	}

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

	void ModuleManager::LoadModules()
	{
		m_dllLoader.LoadEngineDll();
	}

	void ModuleManager::UnloadModules()
	{
		m_dllLoader.UnLoadEngineDll();
	}

	void ModuleManager::InitModules(Factory* factory)
	{
		for (auto module : m_modules)
		{
			if(!module)
			{
				continue;
			}

			module->InitModule(*factory);
		}
	}

}// ishak