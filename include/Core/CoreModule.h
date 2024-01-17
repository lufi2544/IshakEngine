#pragma once

#include "Module/Module.h"

namespace ishak {

	class CoreModule : public Module
	{
	public: 		
		CoreModule();
				
		void DoInitModule(Factory& factory) override;
	};

	REGISTER_MODULE(CoreModule);
	
}// ishak