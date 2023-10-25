
// ISHAK
#include "Functionality/DynamicLibraryLoader.h"


namespace ishak{
	
	// Loads the Dlls for the Engine Modules, if we find a .cpp in a Module folder, we assume it needs to load its .dll
	// Modules inside the ThirdParty dir are skipped.	
	void DllLoader::LoadEngineDlls()
	{			
		// We assume the visual studio project file is ALWAYS gonna be at // Intemediate/ProjectFiles/IshakEngine.vcxproj

		// Figure out if we are running the .exe or the VS project file.
		fs::path currentDir{ fs::current_path() };
		std::string currentDirString{ currentDir.string() };
		fs::path engineRootDir;

		// Running from .exe
		if(currentDirString.find("Binaries") != std::string::npos)
		{
			engineRootDir = currentDir.parent_path();			
		}
		else 
		{
			// Loading from .vcxprj( VS project file ) located in the IntermediateFolder
			engineRootDir = currentDir.parent_path().parent_path();
		}		
		
		std::string modulesDir{ engineRootDir.string() + "\\" + "Source" + "\\" + "Modules" };
		
		TArray<std::string> modulesToLoad;
		ExploreModulesToLoad(modulesDir,  &modulesToLoad);
		LoadModulesDlls(modulesToLoad);				
	}

	void DllLoader::UnLoadEngineDlls()
	{
		for (const HMODULE& module : m_LoadedModules)
		{
			if (module)
			{
				FreeLibrary(module);
			}
		}
	}

	void DllLoader::ExploreModulesToLoad(const std::string& modulesDir, TArray<std::string>* out_ModulesToLoad)
	{		
		// Save the Dll to load if any .cpp found and is not Third Party dir.				
		for (const fs::directory_entry& moduleDir : fs::directory_iterator(modulesDir))
		{
			// Do not explore the ThirdParty dir.
			if (moduleDir.path().filename().string().find("ThirdParty") != std::string::npos)
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
		for (const std::string& moduleName : modules)
		{
			HMODULE loadedModule = LoadLibrary(moduleName.c_str());
			if (loadedModule)
			{
				m_LoadedModules.Add(loadedModule);
			}
		}
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