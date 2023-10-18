#include "Window/Window.h"
#include "SDL/SDL_image.h"
#include <filesystem>


namespace ishak {

	Window::Window(SDL_Window* SDLWindowParam, SDL_Renderer* windowsSDLRendererParam, const WindowCreationContext& creationContext) 
		: m_SDLWindow{ SDLWindowParam }
		, m_ThisWindowRenderer{ windowsSDLRendererParam }
		, m_CreationContext{ creationContext }
	{

	}

	Window::~Window() 
	{
		// Destroy the SDL context for this window.
		SDL_DestroyRenderer(m_ThisWindowRenderer);
		SDL_DestroyWindow(m_SDLWindow);
	}

	SharedPtr<Window> Window::MakeWindow(const WindowCreationContext& creationContext)
	{

		// Centralized Window
		// TODO FUNCTION
		bool bCentralizedWindow{ (creationContext.flags & WindowFlags::WINDOW_CENTRALIZED) != 0 };

		bool bFullScreen{ (creationContext.flags & WindowFlags::WINDOW_FULL_SCREEN) != 0 };
		bool bFullScreenMatchMonitor{ (creationContext.flags & WindowFlags::WINDOW_FULLSCREEN_MATCH_MONITOR) != 0 };
		int windowHeightToDisplay, windowWidthToDisplay;		

		windowWidthToDisplay = creationContext.width;
		windowHeightToDisplay = creationContext.height;

		// Check the Flags for passing to SDL.
		Uint32 SDLWindowFlags = 0;		
		if (bFullScreenMatchMonitor) 
		{
			SDLWindowFlags |= SDL_WINDOW_FULLSCREEN;

			SDL_DisplayMode displayMode;
			SDL_GetCurrentDisplayMode(0, &displayMode);

			windowWidthToDisplay = displayMode.w;
			windowHeightToDisplay = displayMode.h;
		}
		else if (bFullScreen) 
		{
			SDLWindowFlags |= SDL_WINDOW_FULLSCREEN;
		}

		SDL_Window* SDLWindow = SDL_CreateWindow(
			creationContext.title,
			bCentralizedWindow ? SDL_WINDOWPOS_CENTERED : creationContext.x,
			bCentralizedWindow ? SDL_WINDOWPOS_CENTERED : creationContext.y,
			windowWidthToDisplay,
			windowHeightToDisplay,
			// For now no extra flags
			SDLWindowFlags);

		if (!SDLWindow)
		{
			// TODO ERROR
			return nullptr;
		}

		Uint32 extraRendererFlags = 0;
		if ((creationContext.flags & WindowFlags::WINDOW_VSYNC) != 0 ) 
		{
			extraRendererFlags |= SDL_RENDERER_ACCELERATED;
			extraRendererFlags |= SDL_RENDERER_PRESENTVSYNC;
		}
		SDL_Renderer* SDLWindowRenderer = SDL_CreateRenderer(SDLWindow, -1, extraRendererFlags);

		if (!SDLWindowRenderer) 
		{
			// TODO ERROR
			return nullptr;
		}

		return std::make_shared<ishak::Window>(SDLWindow, SDLWindowRenderer, creationContext);		
	}

	void Window::Render() 
	{		
		RenderColor(21, 21, 21, 255);
		SDL_RenderClear(m_ThisWindowRenderer);
		
		/*
		RenderColor(255, 255, 255, 255);
		SDL_Rect player{ 10, 10, 200, 200 };
		SDL_RenderFillRect(m_ThisWindowRenderer, &player);
		
		

		SDL_Surface* tankSurface = IMG_Load("../../Content/Tank_Right.png");
		SDL_Texture* tankTexture = SDL_CreateTextureFromSurface(m_ThisWindowRenderer, tankSurface);
		SDL_FreeSurface(tankSurface);

		SDL_Rect destRect{ m_PlayerX, m_PlayerY, 32, 32 };
		SDL_RenderCopy(m_ThisWindowRenderer, tankTexture, NULL, &destRect);

		SDL_DestroyTexture(tankTexture);

		// Present the Final buffer to the screen.
		SDL_RenderPresent(m_ThisWindowRenderer);
		*/
	}

	void Window::RenderColor(int R, int G, int B, int A)
	{
		SDL_SetRenderDrawColor(m_ThisWindowRenderer, R, G, B, A);

	}

}// ishak
