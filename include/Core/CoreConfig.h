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
		#ifdef ENGINE_EXPORTS
			#define CORE_API _declspec(dllexport)
		#endif
	#endif // CORE_LIB

	#ifndef CORE_API
		#define CORE_API
	#endif
#endif // CORE_API 
