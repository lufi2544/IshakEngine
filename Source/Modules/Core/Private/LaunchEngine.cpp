#include "windows.h"
#include <iostream>

#include "GameLoop.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{			
	ishak::GameLoop gameLoop;
	gameLoop.Init();
	gameLoop.Execute();
		
	return 0;
}