#pragma once
#include "Module/Module.h"

namespace ishak{

	class RendererModule : public Module
	{
	protected:
		void DoInitModule(Factory& factory);
	};

	REGISTER_MODULE(RendererModule);

}// ishak