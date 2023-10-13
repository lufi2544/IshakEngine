#pragma once
#include "CoreMinimal.h"
#include "CoreConfig.h"

namespace ishak {

	/* 
	*  This is the main Engine class.This will control the flow of the
	* initialization for modules, global managers, etc. When adding the Editor
	* I might interface this? I have to figure it out.
	*/
	class CORE_API IshakEngine
	{
	public:
		IshakEngine();
		static int Execute();

	private:
		void Init();				
		void HandleModules();
		void Run();
		void ShutDown();
	};
		
	extern IshakEngine* GEngine;
}// ishak