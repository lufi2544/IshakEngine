#pragma once
#if LINUX
	#if ECS_LIB
		#define ECS_API
	#else 
		#define ECS_API __attribute__((visibility("default")))
	#endif // ECS_LIB
#else
	#if ECS_LIB
	    #define ECS_API __declspec(dllimport)
	#else 
#ifdef ENGINE_EXPORTS
#define ECS_API _declspec(dllexport)
#endif
	#endif // ECS_LIB
#ifndef ECS_API 
#define ECS_API
#endif
#endif // ECS_API
