#pragma once

#include "RendererConfig.h"
#include "CoreMinimal.h"
#include "Asset/Texture.h"
#include "EntityId.h"


namespace ishak{

	struct RENDERER_API RendererCommand
	{
		static RendererCommand FromColor(Ecs::EntityId who, const Vector4& color)
		{
			RendererCommand command;
			command.entityId = who;
			command.color = color;

			return command;
		}

		static RendererCommand FromTexture(Ecs::EntityId who, Texture*  texturePtr, const Vector2& positionParam)
		{
			RendererCommand command;
			command.entityId = who;
			command.texture = texturePtr;
			command.position = positionParam;

			return command;
		}

		Ecs::EntityId entityId{ Ecs::kNullId };
		Texture* texture{ nullptr };
		Vector4 color;
		Vector2 position;
	};

}// ishak