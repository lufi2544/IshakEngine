#include "Level/LevelBackGround_Miscellaneous.h"

#include "Ecs/Components/TextureComponent.h"
#include "Ecs/Components/TransformComponent.h"

namespace ishak
{
	LevelBackGround_Miscellaneous::LevelBackGround_Miscellaneous(
		Ecs::EcsContext* ecsContext,
		WeakPtr<Texture> levelMiscellaneousTexture,
		const Vector2& tiledimensions)
	{
		CreateTilesFromMiscellaneousTexture(ecsContext, levelMiscellaneousTexture, tiledimensions);
	}

	void LevelBackGround_Miscellaneous::CreateTilesFromMiscellaneousTexture(
		Ecs::EcsContext* ecsContext,
		WeakPtr<Texture> levelMiscellaneousTexture,
		const Vector2& tiledimensions)
	{		
		// Create Tiles

		assert(tiledimensions.x != 0);
		assert(tiledimensions.y != 0);
		// Iterate through all the global texture and according to dimensions, create tiles.
		const int textureWitdth{ levelMiscellaneousTexture.lock()->GetWidth() };
		const int textureHeight{ levelMiscellaneousTexture.lock()->GetHeight() };

		Ecs::EntityManager* entityManager{ ecsContext->GetEntityManager() };
		Ecs::ComponentManipulator* componentManip{ ecsContext->GetComponentManipulator() };

		for (int y = 0; y < textureHeight; y += tiledimensions.y)
		{
			for (int x = 0; x < textureWitdth; x += tiledimensions.x)
			{
				// Create a tile with a Texture component with the coordinates of the global texture for later rendering.
				SharedPtr<RenderingTile> renderingTile{ std::make_shared<RenderingTile>() };
				Ecs::EntityId tileEntityId{ entityManager->RegisterEntity(renderingTile.get()) };
				renderingTile->SetEntityId(tileEntityId);

				TextureComponent renderingTileTextureComponent{ NULL, levelMiscellaneousTexture, Vector2{ x, y }, (int)tiledimensions.x, (int)tiledimensions.y };
				componentManip->AddComponentDeferred<TextureComponent>(tileEntityId, renderingTileTextureComponent);
				componentManip->AddComponentDeferred<TransformComponent>(tileEntityId, TransformComponent{ Vector2{ x, y } });


				m_renderingTiles.Add(renderingTile);
			}
		}
	}
}//ishak