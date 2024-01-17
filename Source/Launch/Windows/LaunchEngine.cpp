
#ifdef LINUX
	#include <unistd.h>
#endif

#include "Engine/GameFramework/World.h"
#include "Launch/EngineLoop.h"

#include "Launch/LaunchConfig.h"

LAUNCH_API int main(int arg, char* argv[])
{			

	ishak::EngineLoop engineLoop;
	engineLoop.Init();
	return engineLoop.TickEngine();
}
