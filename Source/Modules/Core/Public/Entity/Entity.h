#pragma once

#include "CoreMinimal.h"
#include "CoreConfig.h"
#include "Functionality/Factory.h"

namespace ishak {
	class World;
}

namespace ishak 
{
	/*** Main class for the Ishak Engine, for now, nothing fancy, for now... */
	class CORE_API Entity 
	{
	public:
		Entity() = default;
		Entity(WeakPtr<World> world);	

		void BeginPlay();

	protected:
		virtual void DoOnBeginPlay(){ }

	private:
		WeakPtr<World> m_World;
	};	
} // ishak