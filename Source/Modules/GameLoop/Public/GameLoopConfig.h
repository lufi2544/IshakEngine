#pragma once
#ifdef GAMELOOP_LIB
    #define GAMELOOP_API __declspec(dllimport)
#else 
    #define GAMELOOP_API __declspec(dllexport)    

#endif // GAMELOOP_LIB