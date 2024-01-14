
// ISHAK
#include "Functionality/DynamicLibraryLoader.h"
#include "FileSystem/FileSystem.h"
#if LINUX
	#include <dlfcn.h>
#endif // LINUX

#if WINDOWS
#include <libloaderapi.h>
#endif //WINDOWS


namespace ishak{
	
	// Loads the Dlls for the Engine Modules, if we find a .cpp in a Module folder, we assume it needs to load its .dll
	// Modules inside the ThirdParty dir are skipped.	
	void DllLoader::LoadEngineDlls()
	{			
		// We assume the visual studio project file is ALWAYS gonna be at // Intemediate/ProjectFiles/IshakEngine.vcxproj

		std::cout << "--Loading Engine Module Dlls--" << std::endl;

		const String&  modulesDir  = FileSystem::Get().GetModulesDir();
		
		TArray<std::string> modulesToLoad;
		ExploreModulesToLoad(std::string{ modulesDir.c_str() },  &modulesToLoad);
		LoadModulesDlls(modulesToLoad);				
	}

	void DllLoader::UnLoadEngineDlls()
	{
#if LINUX
		for(void* module : m_LoadedModules)
		{
			dlclose(module);
		}
	

#else // WINDOWS
		for (const HMODULE& module : m_LoadedModules)
		{
			if (module)
			{
				FreeLibrary(module);
			}
		}
#endif		

	}

	void DllLoader::ExploreModulesToLoad(const std::string& modulesDir, TArray<std::string>* out_ModulesToLoad)
	{		
		// Save the Dll to load if any .cpp found and is not Third Party dir.				
		for (const fs::directory_entry& moduleDir : fs::directory_iterator(modulesDir))
		{

			// Do not explore the ThirdParty dir.
			if (moduleDir.path().filename().string().find("ThirdParty") != std::string::npos || !moduleDir.is_directory())
			{
				continue;
			}


			// Single Module dir
			for (const fs::directory_entry& moduleEntry : fs::directory_iterator(moduleDir.path().string()))
			{
				// We should not have any file except the .Moudule.cs inside the Module Root Dir.
				// E.g
				// - Core -> Private/Public/Core.Module.cs
				if (!moduleEntry.is_directory())
				{
					continue;
				}

				// Try find .cpp
				if (HasAnyCpp(moduleEntry))
				{
					// Taking the Current dir where we found the .cpp (Private module dir) and then accessing the parent dir which 
					// is the module path itself.
					const std::string moduleName = moduleEntry.path().parent_path().filename().string();
					out_ModulesToLoad->AddUnique(moduleName);
				}
			}
		}
	}

	void DllLoader::LoadModulesDlls(const TArray<std::string>& modules)
	{
#if LINUX

	TArray<std::string> modulesToLoad;
	for(const std::string& moduleName : modules)
	{
		
	    const std::string linuxModuleName = "lib" + moduleName + ".so";
		modulesToLoad.Add(linuxModuleName);	
	}

	const String& binariesDir = FileSystem::Get().GetBinariesDir();
	for(const std::string& moduleName : modulesToLoad)
	{
		const String linuxModuleDllPath = binariesDir + moduleName.c_str();
		void* loadedModule = dlopen(linuxModuleDllPath.c_str(), RTLD_NOW | RTLD_GLOBAL);
		if(loadedModule)
		{
			std::cout << "Loaded Module: " << moduleName << std::endl;			
		}else
		{
			std::cerr << "Error handling module" << moduleName << ": " << dlerror() << std::endl;
		}


	}

#elif WINDOWS // WINDOWS

	for (const std::string& moduleName : modules)
	{
		HMODULE loadedModule = LoadLibrary((LPCWSTR)(moduleName.c_str()));
		if (loadedModule)
		{
			m_LoadedModules.Add(loadedModule);
		}
	}

#endif

	}

	bool DllLoader::HasAnyCpp(const fs::directory_entry& dir)
	{
				
		for (const fs::directory_entry& moduleFile : fs::directory_iterator(dir))
		{
			// find .cpp either in this dir or on a child one.
			if(!moduleFile.is_directory())
			{
				const bool bFileIsCpp{ moduleFile.path().string().find(".cpp") != std::string::npos };
				if(bFileIsCpp)
				{
					return true;
				}
			}else
			{
				// Explore is dir has cpp
				bool bDirHasCpp{  HasAnyCpp(moduleFile) };
				if(bDirHasCpp)
				{
					return true;
				}
			}
		}

		return false;
	}


}// ishak
