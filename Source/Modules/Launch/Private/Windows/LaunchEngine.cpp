#include "windows.h"

#include "EngineLoop.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{			
	ishak::EngineLoop engineLoop;
	engineLoop.Init();
	return engineLoop.TickEngine();	
}