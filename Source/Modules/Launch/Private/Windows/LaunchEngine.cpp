//#include "windows.h"
#include "Functionality/Factory.h"
//#include "WinBase.h"

#include <unistd.h>

#include "EngineLoop.h"
#include "Log/Logger.h"

#if LINUX
	int main(int arg, char* argv[])
#else // WINDOWS
	int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#endif // LINUX
{			

	int* ptr = new int(4);
	delete ptr;
	ishak::EngineLoop engineLoop;
	engineLoop.Init();
	return engineLoop.TickEngine();	
}
