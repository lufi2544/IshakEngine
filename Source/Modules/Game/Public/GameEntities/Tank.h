#pragma once

#include "GameFramework/Entity.h"
#include "GameConfig.h"

namespace ishak {

	class GAME_API Tank : public Entity
	{
	public:
		Tank() = default;
		Tank(World* world);

		void Fire();

	protected:
		void DoOnBeginPlay() override;
		void DoUpdate(float deltaTime) override;
	};

}// ishak