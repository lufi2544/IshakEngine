#include "windows.h"
#include <iostream>

#include "Array.h"
#include "GameLoop.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{			

	ishak::test();

	ishak::GameLoop gameLoop;
	gameLoop.Init();
	gameLoop.Execute();
		
	return 0;
}