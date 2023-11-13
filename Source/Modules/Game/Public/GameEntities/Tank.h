#pragma once

#include "GameFramework/Entity.h"
#include "GameConfig.h"

namespace ishak {

	class GAME_API Tank : public Entity
	{
	public:
		Tank() = default;
		Tank(World* world); 
		Tank(World* world, Vector2 position);

		void Fire();

	protected:
		void DoOnBeginPlay() override;
		void DoTick(float deltaTime) override;
	};

}// ishak