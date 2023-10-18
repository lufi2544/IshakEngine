#pragma once

// ISHAK
#include "EngineConfig.h"
#include "CoreMinimal.h"
#include "GameFramework/World.h"

namespace ishak{

	class ENGINE_API GameInstance
	{
	public:
		GameInstance() = default;
		~GameInstance() = default;


		void Init();
		void SetWorld(World* world);

		World* GetWorld();
		World* GetWorld() const;
		
	protected:
		virtual void DoInit(){ }
		
	private:
		/** Main world instance. */
		World* m_world{ nullptr };
	};

}// ishak