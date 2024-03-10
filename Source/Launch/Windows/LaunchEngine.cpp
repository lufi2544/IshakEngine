
#ifdef LINUX
	#include <unistd.h>
#endif

#include "Launch/EngineLoop.h"


LAUNCH_API int main(int arg, char* argv[])
{			
	ishak::EngineLoop engineLoop;
	engineLoop.Init();
	return engineLoop.TickEngine();
}
