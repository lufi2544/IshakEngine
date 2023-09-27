
// Ishak
#include "GameLoop.h"
#include "Window/Window.h"

// STD
#include <iostream>

//Other
#include "SDL.h"

namespace ishak{


	GameLoop::GameLoop() 
	{
		
	}

	GameLoop::~GameLoop() 
	{
		SDL_Quit();
	}

	void GameLoop::Init() 
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
		{
			std::cerr << "Error initializing SDL." << std::endl;
			return;
		}

		ishak::WindowCreationContext winCreationContext = WindowCreationContext
		{
			"IshakEngine",
			0, 0,// where
			800, 600, // dimensions
			WindowFlags::WINDOW_CENTRALIZED  |  WindowFlags::WINDOW_VSYNC
		};

		m_GameMainWindow = ishak::Window::CreateWindow(winCreationContext);				
	}

	void GameLoop::Execute()
	{
		while (!bWantsToExit) 
		{
			ProcessInput();
			Update();
			Render();
		}
	}

	void GameLoop::ProcessInput() 
	{
		SDL_Event sdlEvent;
		while (SDL_PollEvent(&sdlEvent)) 
		{
			switch (sdlEvent.type)
			{
			case SDL_QUIT:
				bWantsToExit = true;

			break;
			case SDL_KEYDOWN:
				if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) 
				{
					bWantsToExit = true;
				}
				break;

			default: 
				break;
			}
		}
	}

	void GameLoop::Render()
	{
		if (m_GameMainWindow) 
		{
			m_GameMainWindow->Render();
		}
	}

	void GameLoop::Update() 
	{

	}


} // ishak;