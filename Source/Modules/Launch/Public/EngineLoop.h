#pragma once
#include "LaunchConfig.h"
#include "CoreMinimal.h"

namespace ishak {

    class Window;
}

namespace ishak {

    class LAUNCH_API EngineLoop
    {
    public:
        EngineLoop();
        ~EngineLoop();

        void Init();
        int TickEngine();        


    private:
        void FinishProgram();
    };

};