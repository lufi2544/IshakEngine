#pragma once

// ISHAK
#include "EngineConfig.h"
#include "CoreMinimal.h"
#include "GameFramework/World.h"
#include "Ecs.h"

namespace ishak{

	class ENGINE_API GameInstance
	{
	public:
		GameInstance() = default;
		~GameInstance() = default;


		void Init();
		void SetWorld(World* world);
		void SetEcsContext(TArray<SharedPtr<Ecs::EcsContext>>* context);
		void ShutDown();

		World* GetWorld();
		World* GetWorld() const;		
		TArray<SharedPtr<Ecs::EcsContext>>* GetEcsContext()
		{
			return m_ecsContextPtr;
		}
		
	protected:
		virtual void DoInit(){ }
		
	private:
		/** Main world instance. */
		World* m_world{ nullptr };

		//TODO Change WorkAround for now
		TArray<SharedPtr<Ecs::EcsContext>>* m_ecsContextPtr{ nullptr };
	};

}// ishak