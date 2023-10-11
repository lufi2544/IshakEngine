#pragma once

// ISHAK
#include "CoreConfig.h"
#include "CoreMinimal.h"
#include "Entity/Entity.h"


namespace ishak {


	class CORE_API World 
	{
	public:
		World() = default;		

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

		/** Entire World Entities, for now handled here. */
		TArray<std::shared_ptr<Entity>> m_WorldEntities;
	};

}// ishak