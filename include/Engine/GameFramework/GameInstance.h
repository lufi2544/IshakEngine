#pragma once

// ISHAK
#include "Engine/EngineConfig.h"
#include "Core/CoreMinimal.h"
#include "Engine/GameFramework/World.h"
#include "Ecs/Ecs.h"

namespace ishak{

	class ENGINE_API GameInstance
	{
	public:
		GameInstance() = default;
		~GameInstance() = default;


		void Init();
		void SetWorld(World* world);
		void SetEcsContext(Ecs::EcsContextContainer* contextContainer);		
		void ShutDown();

		World* GetWorld();
		World* GetWorld() const;		

		Ecs::EcsContext* GetEcsContext(Ecs::ContextID contextId)
		{
			return m_ptrEcsContextContainer->GetEcsContext(contextId);
		}
		
	protected:

		Ecs::EcsContext* CreteCustomEcsContext(SharedPtr<Ecs::EntityManager> entityManager);
		void RegisterEcsGameContainers(Ecs::ComponentManipulator* compMan);
		virtual void DoRegisterEcsGameContainers(Ecs::ComponentManipulator* compMan){  }

		void RegisterEcsGameSystems(Ecs::EcsContext* ecsContext);
		virtual void DoRegisterEcsGameSystems(Ecs::EcsContext* ecsContext) {  }

		virtual void DoInit(){ }
		virtual void DoInitEcsContext(Ecs::EcsContext* gameEcsContext){ }
		
	private:
		/** Main world instance. */
		World* m_world{ nullptr };

		/** Ptr to the Engine Ecs ContextContainer. */
		Ecs::EcsContextContainer* m_ptrEcsContextContainer;
	};

}// ishak