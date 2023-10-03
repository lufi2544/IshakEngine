#pragma once
#ifdef CORE_LIB
#define CORE_API __declspec(dllimport)
#else 
#define CORE_API __declspec(dllexport)    

#endif // GAMELOOP_LIB