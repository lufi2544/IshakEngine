#include "TanksGameInstance.h"
#include "GameEntities/Tank.h"

namespace ishak {

	void TanksGameInstance::DoInit()
	{				
		auto createdTank = GetWorld()->SpawnEntity<Tank>(GetWorld());				
		createdTank.lock()->SetPosition({ 40, 40 });

		auto createdTank1 = GetWorld()->SpawnEntity<Tank>(GetWorld());
		createdTank1.lock()->SetPosition({ 80, 40 });

		auto createdTank2 = GetWorld()->SpawnEntity<Tank>(GetWorld());
		createdTank2.lock()->SetPosition({ 120, 40 });
		
	}
	
}// ishak
