#pragma once

#include "CoreMinimal.h"
#include "RendererConfig.h"
#include "Ecs.h"

#include "Asset/Texture.h"

namespace ishak
{

	/** Info for the creation of a RenderingTile. */
	struct RENDERER_API RenderingTileInfo
	{		
		RenderingTileInfo() = default;

		Vector2 renderingPosition{ -1, -1 };
		Vector2 tileDimensions{ -1, -1 };		
	};

	class RENDERER_API RenderingTile
	{
	public:
		RenderingTile() = default;


		void SetEntityId(Ecs::EntityId entityId)
		{
			m_entityId = entityId;
		}

		Ecs::EntityId GetEntityId() const { return m_entityId; }

	private:
		Ecs::EntityId m_entityId{ Ecs::kNullId };
	};
}//ishak