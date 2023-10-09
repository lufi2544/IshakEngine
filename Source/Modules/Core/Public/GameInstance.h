#pragma once

// ISHAK
#include "CoreConfig.h"
#include "CoreMinimal.h"
#include "World.h"

namespace ishak{

	class CORE_API GameInstance
	{
	public:
		GameInstance() = default;
		~GameInstance() = default;


		void Init();

		std::weak_ptr<World> GetWorld();
		std::weak_ptr<World> GetWorld() const;
		
	protected:
		virtual void DoInit(){ }
		
	private:
		/** Main world instance. */
		std::shared_ptr<World> m_world;
	};

}// ishak