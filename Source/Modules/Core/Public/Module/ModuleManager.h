#pragma once

// ishak
#include "CoreConfig.h"
#include "Module.h"
#include "CoreMinimal.h"
#include "Functionality/DynamicLibraryLoader.h"

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

		~ModuleManager();

		static ModuleManager& Get();

		/** Registers a Module. Used by the Module class to register itsef. */
		void RegisterModule(Module* moduleToAdd);

		/** Loads the Modules into memory. */
		void LoadModules();

		/** Unloads Modules from memory.  */
		void UnloadModules();

		void InitModules(Factory* factory);
		
	   /* Container for the engine modules.For now they are statically allocated
		* when starting the application and a pointer is passed from the instance itself -> (this)
		* so no need for deletion.*/
		TArray<Module*> m_modules;

	private:
		ModuleManager() = default;
		

	private:
		static ModuleManager* m_singleton;
	
	private:
		DllLoader m_dllLoader;
	};
		
}// ishak