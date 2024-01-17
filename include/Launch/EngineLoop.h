#pragma once
#include "LaunchConfig.h"
#include "Core/CoreMinimal.h"

namespace ishak {

    class Window;
}

namespace ishak {

    class LAUNCH_API EngineLoop
    {
        const int FPS = 60;        
        const float FIXED_DELTA = (float)1 / (float)FPS;

    public:
        EngineLoop();
        ~EngineLoop();

        void Init();
        int TickEngine();        


    private:
        void FinishProgram();

        uint64 milisecondsPrevFrame{ 0 };
    };

};