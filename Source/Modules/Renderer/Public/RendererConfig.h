#pragma once
#ifdef RENDERER_LIB
    #define RENDERER_API __declspec(dllimport)
#else 
    #define RENDERER_API __declspec(dllexport)    

#endif // RENDERER_API