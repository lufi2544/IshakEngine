#pragma once

#include "CoreMinimal.h"
#include "EngineConfig.h"

#include "Ecs.h"

#include "Level/LevelBackGround.h"

namespace ishak
{
	class World;	

	class ENGINE_API Level
	{
	public:
		Level(World* world);
		void Init(const String& backgroundImageName);
	private:
		World* m_world{ nullptr };
		
		UniquePtr<LevelBackGround> m_levelBackground;
	};
}// ishak