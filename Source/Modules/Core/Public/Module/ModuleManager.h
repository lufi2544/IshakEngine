#pragma once

// ishak
#include "CoreConfig.h"
#include "Module.h"
#include "CoreMinimal.h"

// STD
#include <string>

namespace ishak
{
	/** Global Manager for all the modules in the game. Each Module will register itself. */
	class CORE_API ModuleManager
	{
	public:		
		ModuleManager(const ModuleManager&) = delete;
		ModuleManager(ModuleManager&&) = delete;
		ModuleManager(ModuleManager&) = delete;

		static ModuleManager& Get();

		void RegisterModule(Module* moduleToAdd);
		void InitModules(Factory* factory);
		
		TArray<Module*> m_modules;

	private:
		ModuleManager() = default;

	private:
		static ModuleManager* m_singleton;
	};
		
}// ishak