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
		Entity(World* world);	

		void BeginPlay();
		void Update(float deltaTime);

	protected:
		virtual void DoOnBeginPlay(){ }
		virtual void DoUpdate(float deltaTime){ }

	private:
		World* m_World{ nullptr };
	};	
} // ishak