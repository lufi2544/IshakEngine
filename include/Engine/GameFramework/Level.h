#pragma once

#include "Core/CoreMinimal.h"
#include "Engine/EngineConfig.h"

#include "Ecs/Ecs.h"

#include "Renderer/Level/LevelBackGround.h"

namespace ishak
{
	class World;	

	class ENGINE_API Level
	{
	public:
		Level(World* world, String const& name);
		void Init(const String& backgroundImageName);
	private:
		World* m_world{ nullptr };
		
		UniquePtr<LevelBackGround> m_levelBackground;
		String m_name;
	};
}// ishak