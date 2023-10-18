#include "TanksGameInstance.h"
#include "GameEntities/Tank.h"

namespace ishak {

	void TanksGameInstance::DoInit()
	{				
		auto createdTank = GetWorld()->SpawnEntity<Tank>(GetWorld());
				
		createdTank.lock()->Fire();
	}
	
}// ishak
