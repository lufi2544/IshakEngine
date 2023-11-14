#pragma once

#include "CoreMinimal.h"
#include "RendererConfig.h"

#include "RenderingTile.h"
#include "LevelBackGround.h"


namespace ishak
{

	class RENDERER_API LevelBackGround_Miscellaneous : public LevelBackGround
	{
	public:		
		LevelBackGround_Miscellaneous(
			Ecs::EcsContext* ecsContext,
			WeakPtr<Texture> levelMiscellaneousTexture,
			const Vector2& tiledimensions);

		~LevelBackGround_Miscellaneous() = default;

	private:
		void CreateTilesFromMiscellaneousTexture(
			Ecs::EcsContext* ecsContext,
			WeakPtr<Texture> levelMiscellaneousTexture,
			const Vector2& tiledimensions);

	private:

		/**
		* Different collection of Rendering Tiles. Each Rendering tile will represent a portion of a global image
		* that contains the whole possible rendering tiles divided in little images. ( Just some cordinates that will take a part of the image, this part 
		* is a tile itself.) 
		*/		
		TArray<SharedPtr<RenderingTile>> m_renderingTiles;
	};
}//ishak