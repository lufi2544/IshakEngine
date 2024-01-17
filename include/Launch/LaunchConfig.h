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
#ifdef ENGINE_EXPORTS
#define LAUNCH_API  _declspec(dllexport)
#endif
	#endif // LAUNCH_LIB

#ifndef LAUNCH_API
#define LAUNCH_API
#endif

#endif // LAUNCH_API
