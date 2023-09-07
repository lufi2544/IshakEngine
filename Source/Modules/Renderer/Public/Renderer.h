#pragma once
#include "RendererConfig.h"


struct RENDERER_API Renderer
{
	Renderer() = default;
	int type;
	int id;

	void PrintRenderer();
};
