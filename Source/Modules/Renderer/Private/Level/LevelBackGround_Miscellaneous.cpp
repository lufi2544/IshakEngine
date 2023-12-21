#include "Level/LevelBackGround_Miscellaneous.h"

#include "Ecs/Components/TextureComponent.h"
#include "Ecs/Components/TransformComponent.h"

#include "FileSystem/FileSystem.h"
#include "RendererTypes.h"

#include <fstream>

namespace ishak
{
	MiscellaneousCreationMap::MiscellaneousCreationMap(String const& creationFileName)
	{
		// TODO Check if this has been created
		const String creationMapPath{ FileSystem::Get().GetAssetsDir() + (creationFileName + ".map") };
				
		std::fstream mapFile;
		mapFile.open(creationMapPath.c_str(), std::ios::in);

		uint8(*fromCharToUint8Number)(char) = [](char from) -> uint8
		{
			// TODO Create NuberTypes Library, 30 is the 0 in the ascii table.
			return from - '0';
		};

		// TODO REFACTOR!!!!
		int width = 0;
		int height = 0;
		bool bShouldUpdate{ true };

		if(mapFile.is_open())
		{
			std::string line;

			while(std::getline(mapFile, line))
			{
				TArray<uint8> mapFileTileId;

				for(int idx = 0; idx < line.size(); ++idx)
				{
					char character{ line[idx] };
					if (character == ',' || idx == line.size() - 1)
					{
						if(idx == line.size() - 1)
						{
							const uint8 number{ fromCharToUint8Number(character) };
							mapFileTileId.Add(number);
						}
						if(bShouldUpdate)
						{
							++width;
							if(idx == line.size() - 1)
							{
								bShouldUpdate = false;
							}
						}
						

						TArray<uint32> toAddNumbers;
						for (uint32 idx = 0; idx < mapFileTileId.Size(); ++idx)
						{
							const uint8 number{ mapFileTileId[idx] };
							if (idx == mapFileTileId.Size() - 1)
							{
								// Last element
								toAddNumbers.Add(number);
							}
							else
							{
								// The rest
								const uint32 numberToAdd{ number * (uint32)(std::pow(10, ((mapFileTileId.Size()) - (idx + 1)))) };
								toAddNumbers.Add(numberToAdd);
							}
				}

						uint32 finalNumber{ 0 };
						for (uint32 number : toAddNumbers)
						{
							finalNumber += number;
						}
						m_tileTypeCollection.Add(finalNumber);
						mapFileTileId.Clear();
						continue;
					}

					const uint8 number{ fromCharToUint8Number(character) };
					mapFileTileId.Add(number);
				}
				bShouldUpdate = false;
				++height;
			}

			mapFile.close();
		}

		m_dimensions = Vector2{ width, height };
	}

	TArray<uint32> const* MiscellaneousCreationMap::GetTileMiscellaneous() const
	{
		return &m_tileTypeCollection;
	}

	LevelBackGround_Miscellaneous::LevelBackGround_Miscellaneous(
		Ecs::EcsContext* ecsContext,
		WeakPtr<Texture> levelMiscellaneousTexture,
		const Vector2& tiledimensions, 
		const MiscellaneousCreationMap& creationMap)
	{
		CreateTilesFromMiscellaneousTexture(ecsContext, levelMiscellaneousTexture, tiledimensions, creationMap);
	}

	void LevelBackGround_Miscellaneous::CreateTilesFromMiscellaneousTexture(
		Ecs::EcsContext* ecsContext,
		WeakPtr<Texture> levelMiscellaneousTexture,
		const Vector2& tiledimensions, 
		MiscellaneousCreationMap const& creationMap)
	{		
		// Create Tile Infos for the different possible tiles in the map
		assert(tiledimensions.x != 0);
		assert(tiledimensions.y != 0);

		// Iterate through all the global texture and according to dimensions, create tiles.
		const int textureWitdth{ levelMiscellaneousTexture.lock()->GetWidth() };
		const int textureHeight{ levelMiscellaneousTexture.lock()->GetHeight() };

		Ecs::EntityManager* entityManager{ ecsContext->GetEntityManager() };
		Ecs::ComponentManipulator* componentManip{ ecsContext->GetComponentManipulator() };

		// Create the Renderingtile infos
		for (int y = 0; y < textureHeight; y += tiledimensions.y)
		{
			for (int x = 0; x < textureWitdth; x += tiledimensions.x)
			{
				m_renderintTileInfos.Add(RenderingTileInfo{ Vector2{ x, y }, Vector2{ (int)tiledimensions.x, (int)tiledimensions.y }});

			}
		}

		// Create the map tiles
		TArray<uint32> miscellaneous{ *creationMap.GetTileMiscellaneous() };
		const Vector2& mapDimensions{ creationMap.GetDimensions() };

		int m = 0;
		for(int tileY = 0; tileY < mapDimensions.y; ++tileY)
		{
			for (int tileX = 0; tileX < mapDimensions.x; ++tileX, ++m)
			{
				const uint32 miscId{ miscellaneous[m] };
				assert(m_renderintTileInfos.CheckAssertSizeAt(miscId));

				RenderingTileInfo& info{ m_renderintTileInfos[miscId] };

				SharedPtr<RenderingTile> renderingTile{ std::make_shared<RenderingTile>() };
				Ecs::EntityId tileEntityId{ entityManager->RegisterEntity(renderingTile.get()) };
				renderingTile->SetEntityId(tileEntityId);

				TextureComponent renderingTileTextureComponent
				{ 
					NULL,
					levelMiscellaneousTexture,
					info.renderingPosition, 
					(int)info.tileDimensions.x, 
					(int)info.tileDimensions.y,
					(uint8)ERendererLayers::Level
				};
				componentManip->AddComponentDeferred<TextureComponent>(tileEntityId, renderingTileTextureComponent);
				componentManip->AddComponentDeferred<TransformComponent>(tileEntityId, TransformComponent{ Vector2{ tileX * tiledimensions.x, tileY * tiledimensions.y } });

				m_renderingTiles.Add(std::move(renderingTile));
			}			
		}

	}	
}//ishak
