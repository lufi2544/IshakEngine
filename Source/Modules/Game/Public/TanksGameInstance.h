#pragma once 

// ISHAK
#include "GameConfig.h"
#include "GameInstance.h"

namespace ishak {

	class GAME_API TanksGameInstance : public GameInstance
	{
	public:
		TanksGameInstance() = default;

	protected:
		void DoInit() override;
	};

}// ishak
