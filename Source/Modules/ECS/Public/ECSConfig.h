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
	    #define ECS_API __declspec(dllexport)
	#endif // ECS_LIB

#endif // ECS_API
