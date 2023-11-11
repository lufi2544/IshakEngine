#pragma once

#include "RendererConfig.h"
#include "CoreMinimal.h"
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

		static RendererCommand FromTexture(Ecs::EntityId who, const String& texturePathParam, const Vector2& positionParam)
		{
			RendererCommand command;
			command.entityId = who;
			command.texturePath = texturePathParam;
			command.position = positionParam;

			return command;
		}

		Ecs::EntityId entityId{ Ecs::kNullId };
		String texturePath;
		Vector4 color;
		Vector2 position;
	};

}// ishak