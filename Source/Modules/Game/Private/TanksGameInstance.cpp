#include "TanksGameInstance.h"
#include "GameEntities/Tank.h"

namespace ishak {

	void TanksGameInstance::DoInit()
	{				
		auto createdTank = GetWorld()->SpawnEntity<Tank>(GetWorld(), Vector2{ 40, 40 }, 0.0f, 33);		
		auto createdTank1 = GetWorld()->SpawnEntity<Tank>(GetWorld(), Vector2{ 80, 40 });
		auto createdTank2 = GetWorld()->SpawnEntity<Tank>(GetWorld(), Vector2{ 120, 40 });		
	}
	
}// ishak
