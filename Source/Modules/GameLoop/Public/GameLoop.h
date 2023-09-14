#pragma once
#include "GameLoopConfig.h"
#include <memory>

namespace ishak {

    class Window;
}

namespace ishak {

    class GAMELOOP_API GameLoop
    {
    public:
        GameLoop();
        ~GameLoop();

        void Init();
        void Execute();
                       
    private:
        void ProcessInput();     
        void Render();
        void Update();
    

    private:
        // TODO Window Manager.
        std::shared_ptr<Window> m_GameMainWindow;
        bool bWantsToExit = false;
    };

};