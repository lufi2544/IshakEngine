#include "Window/Window.h"

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

	std::shared_ptr<Window> Window::CreateWindow(const WindowCreationContext& creationContext)
	{
		Uint32 SDLWindowFlags = 0;		

		// Centralized Window
		// TODO FUNCTION
		bool bCentralizedWindow = creationContext.flags & WindowFlags::WINDOW_CENTRALIZED;		
	
		SDL_Window* SDLWindow = SDL_CreateWindow(
			creationContext.title,
			bCentralizedWindow ? SDL_WINDOWPOS_CENTERED : creationContext.x,
			bCentralizedWindow ? SDL_WINDOWPOS_CENTERED : creationContext.y,
			creationContext.width,
			creationContext.height,
			// For now no extra flags
			0);

		if (!SDLWindow)
		{
			// TODO ERROR
			return nullptr;
		}

		SDL_Renderer* SDLWindowRenderer = SDL_CreateRenderer(SDLWindow, -1, 0);

		if (!SDLWindowRenderer) 
		{
			// TODO ERROR
			return nullptr;
		}

		return std::make_shared<ishak::Window>(SDLWindow, SDLWindowRenderer, creationContext);		
	}

	void Window::Render() 
	{
		RenderColor(255, 0, 0, 255);
		SDL_RenderClear(m_ThisWindowRenderer);
		SDL_RenderPresent(m_ThisWindowRenderer);
	}

	void Window::RenderColor(int R, int G, int B, int A)
	{
		SDL_SetRenderDrawColor(m_ThisWindowRenderer, R, G, B, A);

	}

}// ishak
