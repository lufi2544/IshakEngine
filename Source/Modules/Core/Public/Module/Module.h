#pragma once

#include "CoreConfig.h"
#include "CoreMinimal.h"

#include "Functionality/Factory.h"

namespace ishak {

	/** General macro for defining a module. */
#define REGISTER_MODULE(moduleName) \
    namespace { \
         moduleName _##moduleName; \
    }							\

	/** This is the main class for the Engine Modules. Every Module will register
	*   itself.
	*/
	class CORE_API Module
	{
	public:

		Module();

		virtual ~Module() = default;

		void InitModule(Factory& factory);
		void ShutDownModule();
		
	protected:
		virtual void DoInitModule(Factory& factory) = 0;
		virtual void DoShutDownModule() {};
			
	};

}// ishak