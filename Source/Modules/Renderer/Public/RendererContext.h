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

		static RendererCommand FromTexture(
			Ecs::EntityId who,
			Texture*  texturePtr, 
			const Vector2& positionParam, 
			const float& rotationParam, 
			const int& scaledW, 
			const int& scaledH,
			const Vector2& renderingCoordinates)
		{
			RendererCommand command;
			command.entityId = who;
			command.texture = texturePtr;
			command.position = positionParam;
			command.rotation = rotationParam;
			command.scaledW = scaledW;
			command.scaledH = scaledH;
			command.renderingCoordinates = renderingCoordinates;

			return command;
		}

		Ecs::EntityId entityId{ Ecs::kNullId };
		Texture* texture{ nullptr };
		Vector4 color{ 0, 0, 0, 0};
		Vector2 position{ 0,0 };
		float rotation{ 0.0f };
		int scaledW{ 0 };
		int scaledH{ 0 };
		Vector2 renderingCoordinates{ -1, -1 };
	};

}// ishak