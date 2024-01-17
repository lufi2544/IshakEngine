#pragma once
#include "LaunchConfig.h"
#include "Core/CoreMinimal.h"

namespace ishak {

    class Window;
}

namespace ishak {

    class LAUNCH_API GameLoop
    {
    public:
        GameLoop();
        ~GameLoop();

        void Init();
        void TickEngine();
                       
    private:
        void ProcessInput();     
        void Render();
        void Update(float deltaTime);
    

    private:
        // TODO Window Manager.
        SharedPtr<Window> m_GameMainWindow;
        bool bWantsToExit = false;
    };

};