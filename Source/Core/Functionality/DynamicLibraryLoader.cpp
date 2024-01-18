
// ISHAK
#include "Core/Functionality/DynamicLibraryLoader.h"
#include "Core/FileSystem/FileSystem.h"
#ifdef LINUX
	#include <dlfcn.h>
#endif // LINUX

#ifdef WINDOWS
#include <libloaderapi.h>
#endif //WINDOWS


namespace ishak{
	

	void DllLoader::UnLoadEngineDll()
	{
#ifdef LINUX
		dlclose(m_engineDll);

#else // WINDOWS
		FreeLibrary(m_engineDll);
#endif		

	}

	void DllLoader::LoadEngineDll()
	{
#ifdef LINUX

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
		/*
		std::cout << "Loading Engine Dll..." << std::endl;
		HMODULE loadedDll = LoadLibraryA("IshakEngine.dll");
		if (!loadedDll)
		{
			// TODO ISHException.
			std::cout << "Problem loading Engine Dll." << std::endl;
		}
		else
		{
			m_engineDll = loadedDll;
		}

		*/
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
