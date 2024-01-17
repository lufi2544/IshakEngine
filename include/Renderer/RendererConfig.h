#pragma once

#ifdef LINUX

	#if RENDERER_LIB
		#define RENDERER_API 
	#else
		#define RENDERER_API __attribute__((visibility("default")))
	#endif

#else // WINDOWS

	#ifdef RENDERER_LIB
	    #define RENDERER_API __declspec(dllimport)
	#else 
		#ifdef ENGINE_EXPORTS
			#define RENDERER_API _declspec(dllexport)
		#endif
			#endif
#endif // PLATFORMS


#ifndef RENDERER_API
	#define RENDERER_API
#endif
