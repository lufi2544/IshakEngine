#pragma once
#ifdef LAUNCH_LIB
    #define LAUNCH_API __declspec(dllimport)
#else 
    #define LAUNCH_API __declspec(dllexport)    

#endif // LAUNCH_API