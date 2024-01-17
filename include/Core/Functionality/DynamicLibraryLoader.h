#pragma once

// ISHAK
#include "Core/CoreConfig.h"
#include "Core/CoreMinimal.h"

//Other
// TODO PLATFORM must include only the platform here and let the platform 
// decide I think. Deep deep future.
// TODO LINUX Care about the Loaded DLLs
//#include "Windows.h"

// STD
#include <filesystem>

#if WINDOWS // TODO 
	#include <windows.h>
#endif // WINDOWS

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
		
		void LoadEngineDll();
		void UnLoadEngineDll();
	
	private:
		bool HasAnyCpp(const std::filesystem::directory_entry& dir);
		

	private:

		
#ifdef LINUX
		void* m_engineDll;
#endif // LINUX

#ifdef WINDOWS
	HMODULE m_engineDll;
#endif// WINDOWS

	};

}// ishak
