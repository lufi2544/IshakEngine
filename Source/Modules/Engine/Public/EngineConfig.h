#pragma once
#if LINUX
	#ifdef ENGINE_LIB
		#define ENGINE_API
	#else
		#define ENGINE_API __attribute__((visibility("default")))
	#endif

#else
	#if ENGINE_LIB
	    #define ENGINE_API __declspec(dllimport)
	#else 
	    #define ENGINE_API __declspec(dllexport)    
	#endif
#endif // ENGINE_API
