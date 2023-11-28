//#include "windows.h"
#include "Functionality/Factory.h"
//#include "WinBase.h"

#include <unistd.h>

#include "EngineLoop.h"
#include "Log/Logger.h"

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
int main(int arg, char* argv[])
{			

	ishak::EngineLoop engineLoop;
	engineLoop.Init();
	return engineLoop.TickEngine();	
}
