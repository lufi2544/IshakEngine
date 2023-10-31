#pragma once
#ifdef ECS_LIB
    #define ECS_API __declspec(dllimport)
#else 
    #define ECS_API __declspec(dllexport)    

#endif // ECS_API