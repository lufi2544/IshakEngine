#pragma once


#if LINUX

	#if GAME_LIB
		#define GAME_API
	#else 
		#define GAME_API __attribute__((visibility("default")))
	#endif

#else // WINDOWS

#if GAME_LIB
    #define GAME_API __declspec(dllimport)
#else 
    #define GAME_API __declspec(dllexport)    
#endif

#endif // GAME_API
