#pragma once
#ifdef LINUX
	#ifdef ENGINE_LIB
		#define ENGINE_API
	#else
		#define ENGINE_API __attribute__((visibility("default")))
	#endif

#else
	#ifdef ENGINE_LIB
	    #define ENGINE_API __declspec(dllimport)
	#else 
		#ifdef ENGINE_EXPORTS
			#define ENGINE_API _declspec(dllexport)
		#endif
	#endif
#endif // PLATFORMS

#ifndef ENGINE_API
	#define ENGINE_API
#endif
