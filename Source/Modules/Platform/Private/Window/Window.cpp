#include "Window/Window.h"
#include "SDL/SDL_image.h"
#include <filesystem>


namespace ishak {

	uint16 Window::windowIdGenerator = 0;

	Window::Window(SDL_Window* SDLWindowParam, const WindowCreationContext& creationContext) 
		: m_SDLWindow{ SDLWindowParam }		
		, m_CreationContext{ creationContext }
		, m_windowId{ windowIdGenerator++ }
		, m_color{ 0, 0, 0, 0 }
	{
		
		
	}

	Window::~Window() 
	{
		// Broadcast the Destroyed Delegate.
		if(OnDestoyedDelegate)
		{
			std::invoke(OnDestoyedDelegate);
		}else
		{
			// TODO Exception
		}
		// Destroy the SDL context for this window.
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
			std::cout<<"SLD Error:" << SDL_GetError() << std::endl;
			std::cout << "Error Creating the widow" << std::endl;
			// TODO ERROR
			return nullptr;
		}

		Uint32 extraRendererFlags = 0;
		if ((creationContext.flags & WindowFlags::WINDOW_VSYNC) != 0 ) 
		{
			extraRendererFlags |= SDL_RENDERER_ACCELERATED;
			extraRendererFlags |= SDL_RENDERER_PRESENTVSYNC;
		}

		return std::make_shared<ishak::Window>(SDLWindow, creationContext);		
	}

	void Window::SetOnDestroyedDelegate(std::function<void()>&& delegate)
	{
		OnDestoyedDelegate = std::move(delegate);
	}

}// ishak
