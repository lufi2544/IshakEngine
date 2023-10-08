#pragma once
#ifdef GAME_LIB
    #define GAME_API __declspec(dllimport)
#else 
    #define GAME_API __declspec(dllexport)    

#endif // GAME_API