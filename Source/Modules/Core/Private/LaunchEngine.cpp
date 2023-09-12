#include "windows.h"
#include "Renderer.h"
#include <iostream>
#include "glm/geometric.hpp"
#include "sol/sol.hpp"
#include "SDL.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
//int main(void)
{		
	
	glm::vec<3, float> f{ 3, 3, 3};

	int a = glm::length(f);

	int error = SDL_Init(SDL_INIT_EVENTS);
		
	SDL_Quit();

	sol::state lua;
	lua.open_libraries(sol::lib::base);
		

	return 0;
}