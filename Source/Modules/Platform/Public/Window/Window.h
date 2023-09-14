#pragma once

#include <iostream>
#include "memory"
#include "SDL.h"
#include "PlatformConfig.h"

namespace ishak {

	enum PLATFORM_API WindowFlags : std::uint8_t
	{
		WINDOW_CENTRALIZED = 1 << 7,
	};

	struct PLATFORM_API WindowCreationContext
	{
		WindowCreationContext() = default;
		const char* title;
		int x, y;
		int width, height;
		std::uint8_t flags;
	};

	class PLATFORM_API Window 
	{

	public:
		static std::shared_ptr<Window> CreateWindow(const WindowCreationContext& creationContext);		

		Window(SDL_Window* SDLWindowParam, SDL_Renderer* windowSDLRendererParam, const WindowCreationContext& creationContext);
		~Window();

		void Render();
		void RenderColor(int R, int G, int B, int A);


	private:
		Window() = default;

	private:
		SDL_Window* m_SDLWindow{ nullptr };
		SDL_Renderer* m_ThisWindowRenderer{ nullptr };
		WindowCreationContext m_CreationContext;
	};

}// ishak