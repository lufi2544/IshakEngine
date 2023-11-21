#include "GameFramework/Level.h"

// CORE
#include "Ecs/Components/LevelComponent.h"
#include "FileSystem/FileSystem.h"
#include "Asset/AssetManager.h"
#include "Log/Logger.h"

//GAMEFRAMEWORK
#include "GameFramework/World.h"
#include "GameFramework/GameInstance.h"

#include "Level/LevelBackGround_Miscellaneous.h"


namespace ishak
{
	Level::Level(World* world, String const& name)
		: m_world{ world }
		, m_name{ name }
	{
	}

	void Level::Init(const String& backgroundImage)
	{
		const String backGroundPath{ FileSystem::Get().GetAssetsDir() + backgroundImage };

		// TODO Static method for creating an asset, better to get the asset and if it is not loaded, then create it.

		WeakPtr<Texture> levelTexture{ AssetManager::Get().GetAsset<Texture>(backGroundPath) };
		if(!levelTexture.lock())
		{
			// error trying to load background for level			
			ISHAK_LOG(Error, "error trying to load background for level")
			return;
		}		
		

		MiscellaneousCreationMap creationMap{ m_name };


		m_levelBackground = std::make_unique<LevelBackGround_Miscellaneous>(
			m_world->GetGameInstance().lock()->GetEcsContext(Ecs::ContextID::RENDERER),
			levelTexture,
			Vector2{ 32, 32 }, 
			creationMap);
	}
}//ishak