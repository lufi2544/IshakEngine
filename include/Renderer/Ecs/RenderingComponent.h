#pragma once

#include "Ecs/Ecs.h"
#include "Renderer/RendererConfig.h"
#include "Core/CoreMinimal.h"

namespace ishak
{
	struct RENDERER_API RenderingComponent
	{		

		bool IsValid() const
		{
			constexpr Vector4 defaultValue{ 0, 0, 0, 0 };
			return (color != defaultValue) || ((texturePath != nullptr));			
		}

		String* texturePath{ nullptr };
		Vector4 color{ -1, -1, -1, -1 };		
	};
}
