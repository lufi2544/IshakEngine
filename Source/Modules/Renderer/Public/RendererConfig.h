#pragma once

#if LINUX

	#if RENDERER_LIB
		#define RENDERER_API 
	#else
		#define RENDERER_API __attribute__((visibility("default")))
	#endif

#else // WINDOWS

	#ifdef RENDERER_LIB
	    #define RENDERER_API __declspec(dllimport)
	#else 
	    #define RENDERER_API __declspec(dllexport)    
	#endif
#endif // RENDERER_API
