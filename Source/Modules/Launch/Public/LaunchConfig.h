#pragma once

#ifdef LINUX

	#ifdef LAUNCH_LIB
		#define LAUNCH_API
	#else
		#define LAUNCH_API __attribute__((visibility("default")))
	#endif// LAUNCH_LIB

#else // WINDOWS

	#if LAUNCH_LIB
	    #define LAUNCH_API __declspec(dllimport)
	#else 
	    #define LAUNCH_API __declspec(dllexport)    
	#endif // LAUNCH_LIB

#endif // LAUNCH_API
