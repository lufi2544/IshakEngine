#pragma once

#include "Entity/Entity.h"
#include "GameConfig.h"

namespace ishak {

	class GAME_API Tank : public Entity
	{
	public:
		Tank() = default;
		Tank(WeakPtr<World> world);

		void Fire();

	protected:
		void DoOnBeginPlay() override;
	};

}// ishak