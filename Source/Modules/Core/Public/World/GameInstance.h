#pragma once

// ISHAK
#include "CoreConfig.h"
#include "CoreMinimal.h"
#include "World/World.h"

namespace ishak{

	class CORE_API GameInstance
	{
	public:
		GameInstance() = default;
		~GameInstance() = default;


		void Init();

		WeakPtr<World> GetWorld();
		WeakPtr<World> GetWorld() const;
		
	protected:
		virtual void DoInit(){ }
		
	private:
		/** Main world instance. */
		SharedPtr<World> m_world;
	};

}// ishak