#pragma once

#include <vector>
#include <memory>

namespace ishak {

	class Entity;
}

namespace ishak {


	class World 
	{
	public:
		World() = default;
		~World() = default;

		std::weak_ptr<Entity> SpawnEntity(int x, int y, std::uint8_t type);

		/** Entire World Entities, for now handled here. */
		std::vector<std::shared_ptr<Entity>> m_WorldEntities;
	};

}// ishak