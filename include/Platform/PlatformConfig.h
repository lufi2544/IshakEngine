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
#ifdef ENGINE_EXPORTS
#define PLATFORM_API _declspec(dllexport)
#endif
	#endif // PLATFORM_LIB


#ifndef PLATFORM_API
#define PLATFORM_API
#endif

#endif  // LINUX
