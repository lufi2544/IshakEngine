#include "TanksGameInstance.h"

namespace ishak {

	void TanksGameInstance::DoInit()
	{
		// TODO Factory this
		auto createdEntity = GetWorld().lock()->SpawnEntity(20, 20, 8);


	}
	
}// ishak
