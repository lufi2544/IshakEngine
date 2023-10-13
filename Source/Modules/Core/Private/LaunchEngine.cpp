#include "windows.h"

#include "Engine/IshakEngine.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{			
	return ishak::IshakEngine::Execute();	
}