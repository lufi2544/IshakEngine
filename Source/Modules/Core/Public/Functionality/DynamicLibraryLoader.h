#pragma once

// ISHAK
#include "CoreConfig.h"
#include "CoreMinimal.h"

//Other
// TODO PLATFORM must include only the platform here and let the platform 
// decide I think. Deep deep future.
// TODO LINUX Care about the Loaded DLLs
//#include "Windows.h"

// STD
#include <filesystem>

namespace ishak{
	

	namespace fs = std::filesystem;

	/** Handler in charge of loading the necessary Dlls at any time needed.
	* for now we are gonna use this for the modules initialization.
	*/
	class CORE_API DllLoader
	{
	public:
		DllLoader() = default;
		~DllLoader() = default;
		
		void LoadEngineDlls();
		void UnLoadEngineDlls();
	
	private:
		bool HasAnyCpp(const std::filesystem::directory_entry& dir);
		void ExploreModulesToLoad(const std::string& modulesDir, TArray<std::string>* out_ModulesToLoad);
		void LoadModulesDlls(const TArray<std::string>& modules);
		

	private:
		//TArray<HMODULE> m_LoadedModules;
	};

}// ishak
