#pragma once

#if LINUX 
	#if CORE_LIB
		#define CORE_API
	#else
		#define CORE_API __attribute__((visibility("default")))
	#endif // CORE_LIB

 #else
	#if CORE_LIB
		#define CORE_API __declspec(dllimport)
	 #else 
		#define CORE_API __declspec(dllexport)    
	#endif // CORE_LIB
#endif // CORE_API 
