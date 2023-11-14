#pragma once

#include "CoreMinimal.h"
#include "RendererConfig.h"
#include "Ecs.h"

#include "Asset/Texture.h"

namespace ishak
{
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