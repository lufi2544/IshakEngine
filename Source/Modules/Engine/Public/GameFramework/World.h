#pragma once

// ISHAK
#include "EngineConfig.h"
#include "CoreMinimal.h"
#include "GameFramework/Entity.h"


namespace ishak{

	class GameInstance;
	class Level;
}

namespace ishak {


	class ENGINE_API World 
	{
	public:
		World() = default;	
		World(WeakPtr<GameInstance> GameInstanceParam);

		// TODO Template meta programing to see if the class is derived from Entity, if not, then compile error.
		template<typename T, typename ...Args>
		WeakPtr<T> SpawnEntity(Args&&... args)
		{
			// TODO TEMPLATE Make sure the class is a type of Entity

			SharedPtr<T> createdEntity = std::make_shared<T>(std::forward<Args>(args)...);
			createdEntity->BeginPlay();

			m_WorldEntities.Add(createdEntity);
			return createdEntity;
		}

	public:
		
		template<typename T = GameInstance>
		WeakPtr<GameInstance> GetGameInstance()
		{
			return std::dynamic_pointer_cast<T>(m_GameInstance.lock());
		}

		void Init();
		void Update(float deltaTime);
		void ShutDown();

		template<class FunctionT>
		inline void DoInAllEntities(FunctionT function)
		{
			for(SharedPtr<Entity>& entity : m_WorldEntities)
			{
				function(entity);
			}
		}

	private:
		/** Entire World Entities, for now handled here. */
		TArray<SharedPtr<Entity>> m_WorldEntities;

		/* Game Instance Ptr, this will always exist while the World exists. */
		WeakPtr<GameInstance> m_GameInstance;

		SharedPtr<Level> m_level;
	};

}// ishak