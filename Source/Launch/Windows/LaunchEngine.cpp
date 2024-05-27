
#ifdef LINUX
	#include <unistd.h>
#endif

#include "Launch/EngineLoop.h"
#include "ThirdParty/SDL/SDL_main.h"

extern "C" {
	LAUNCH_API int SDL_main(int arg, char* argv[])
	{
		ishak::EngineLoop engineLoop;
		engineLoop.Init();
		return engineLoop.TickEngine();
	}
}
