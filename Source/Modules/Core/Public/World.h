#pragma once

// ISHAK
#include "CoreConfig.h"
#include "CoreMinimal.h"


namespace ishak {

	class Entity;
}

namespace ishak {


	class CORE_API World 
	{
	public:
		World() = default;
		~World() = default;

		std::weak_ptr<Entity> SpawnEntity(int x, int y, std::uint8_t type);

		/** Entire World Entities, for now handled here. */
		TArray<std::shared_ptr<Entity>> m_WorldEntities;
	};

}// ishak