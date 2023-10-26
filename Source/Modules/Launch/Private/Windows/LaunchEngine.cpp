#include "windows.h"
#include "Functionality/Factory.h"
#include "WinBase.h"

#include "EngineLoop.h"
#include "Log/Logger.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{			
	ishak::EngineLoop engineLoop;
	engineLoop.Init();

	return engineLoop.TickEngine();	
}