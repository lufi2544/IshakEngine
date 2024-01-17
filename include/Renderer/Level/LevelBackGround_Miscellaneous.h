#pragma once

#include "Core/CoreMinimal.h"
#include "Renderer/RendererConfig.h"

#include "RenderingTile.h"
#include "LevelBackGround.h"
#include "Renderer/Asset/Texture.h"


namespace ishak
{

	class RENDERER_API MiscellaneousCreationMap
	{
	public:
		MiscellaneousCreationMap() = default;
		MiscellaneousCreationMap(String const& levelName);

		TArray<uint32> const* GetTileMiscellaneous() const;
		const Vector2& GetDimensions() const
		{
			return m_dimensions;
		}


	private:
		TArray<uint32> m_tileTypeCollection;		
		Vector2 m_dimensions;
	};

	class RENDERER_API LevelBackGround_Miscellaneous : public LevelBackGround
	{
	public:		
		LevelBackGround_Miscellaneous(
			Ecs::EcsContext* ecsContext,
			WeakPtr<Texture> levelMiscellaneousTexture,
			const Vector2& tiledimensions,
			const MiscellaneousCreationMap& creationMap);

		~LevelBackGround_Miscellaneous() = default;

	private:
		void CreateTilesFromMiscellaneousTexture(
			Ecs::EcsContext* ecsContext,
			WeakPtr<Texture> levelMiscellaneousTexture,
			const Vector2& tiledimensions, 
			MiscellaneousCreationMap const& creationMap);

	private:


		/** Infos for creating the RenderingTiles on demand from the creation map. */
		TArray<RenderingTileInfo> m_renderintTileInfos;

		/**
		* Different collection of Rendering Tiles. Each Rendering tile will represent a portion of a global image
		* that contains the whole possible rendering tiles divided in little images. ( Just some cordinates that will take a part of the image, this part 
		* is a tile itself.) 
		*/		
		TArray<SharedPtr<RenderingTile>> m_renderingTiles;
	};
}//ishak