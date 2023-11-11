#pragma once

#include "Ecs.h"
#include "RendererConfig.h"
#include "CoreMinimal.h"

namespace ishak
{
	struct RENDERER_API RenderingComponent
	{		

		bool IsValid() const
		{
			constexpr Vector4 default{ 0, 0, 0, 0 };
			return (color != default) || ((texturePath != nullptr) && (position != nullptr));			
		}

		String* texturePath{ nullptr };
		Vector4 color{ -1, -1, -1, -1 };
		Vector2* position{ nullptr };
	};
}