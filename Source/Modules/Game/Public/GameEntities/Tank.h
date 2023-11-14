#pragma once

#include "GameFramework/Entity.h"
#include "GameConfig.h"

namespace ishak {

	class GAME_API Tank : public Entity
	{
	public:
		Tank() = default;
		Tank(World* world); 

		// TODO Create Transform struct
		Tank(World* world, Vector2 position, float rotation = 0.0f, float scale = 1.0f);

		void Fire();

	protected:
		void DoOnBeginPlay() override;
		void DoTick(float deltaTime) override;
	};

}// ishak