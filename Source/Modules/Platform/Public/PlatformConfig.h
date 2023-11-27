#pragma once
#if LINUX

	#if PLATFORM_LIB
		#define PLATFORM_API
	#else 
		#define PLATFORM_API __attribute__((visibility("default")))
	#endif // PLATFORM_LIB

#else //WINDOWS

	#ifdef PLATFORM_LIB
	    #define PLATFORM_API __declspec(dllimport)
	#else 
	    #define PLATFORM_API __declspec(dllexport)    
	#endif // PLATFORM_LIB

#endif  // LINUX
