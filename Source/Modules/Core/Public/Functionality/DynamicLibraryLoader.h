#pragma once

// ISHAK
#include "CoreConfig.h"
#include "CoreMinimal.h"

//Other
// TODO PLATFORM must include only the platform here and let the platform 
// decide I think. Deep deep future.
#include "Windows.h"

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
		~DllLoader();
		
		void LoadEngineDlls();

	private:
		bool HasAnyCpp(const std::filesystem::directory_entry& dir);


	private:
		TArray<HMODULE> m_LoadedModules;
	};

}// ishak