#pragma once

#include <memory>

namespace ishak {
	class World;
}

namespace ishak 
{
	/*** Main class for the Ishak Engine, for now, nothing fancy, for now... */
	class Entity 
	{
	public:
		Entity() = default;
		~Entity() = default;


	private:
		std::weak_ptr<World> m_World;
	};

} // ishak