#pragma once

#include "SDL.h"
#include "PlatformConfig.h"
#include "CoreMinimal.h"

namespace ishak {

	enum PLATFORM_API WindowFlags : std::uint8_t
	{
		WINDOW_CENTRALIZED = 1 << 7,
		WINDOW_FULL_SCREEN = 1 << 6,
		WINDOW_FULLSCREEN_MATCH_MONITOR = 1 << 5,
		WINDOW_VSYNC = 1 << 4

	};

	struct PLATFORM_API WindowCreationContext
	{
		WindowCreationContext() = default;

		const char* title{ nullptr };
		int x, y; //where
		int width, height; //dimensions
		std::uint8_t flags;
	};

	class PLATFORM_API Window 
	{
		static uint16 windowIdGenerator;
	public:
		static SharedPtr<Window> MakeWindow(const WindowCreationContext& creationContext);		

		Window(SDL_Window* SDLWindowParam, const WindowCreationContext& creationContext);
		~Window();
				
		void SetOnDestroyedDelegate(std::function<void()>&& delegate);
				
		SDL_Window* GetSDLWindow() { return m_SDLWindow; }
		uint16 GetId()const { return m_windowId; }
		const WindowCreationContext& GetCreationContext() const { return m_CreationContext; }
		inline const glm::vec<4, int>& GetColor() { return m_color; };

		std::function<void()> OnDestoyedDelegate;

	private:
		Window() = default;

	private:
		SDL_Window* m_SDLWindow{ nullptr };		
		WindowCreationContext m_CreationContext;
		uint16 m_windowId{ 0 };
		glm::vec<4, int> m_color;
	};

}// ishak