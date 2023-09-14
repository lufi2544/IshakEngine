#pragma once
#ifdef PLATFORM_LIB
    #define PLATFORM_API __declspec(dllimport)
#else 
    #define PLATFORM_API __declspec(dllexport)    

#endif // PLATFORM_LIB