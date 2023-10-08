#pragma once

// ISHAK
#include "CoreConfig.h"
#include "CoreMinimal.h"

namespace ishak{

	class World;
}

namespace ishak{

	class CORE_API GameInstance
	{
	public:
		GameInstance() = default;
		~GameInstance() = default;


		void Init();
		
	protected:
		virtual void DoInit(){ }
		
	private:
		/** Main world instance. */
		std::shared_ptr<World> m_world;
	};

}// ishak