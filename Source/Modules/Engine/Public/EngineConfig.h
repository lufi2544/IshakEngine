#pragma once
#ifdef ENGINE_LIB
    #define ENGINE_API __declspec(dllimport)
#else 
    #define ENGINE_API __declspec(dllexport)    

#endif // ENGINE_API