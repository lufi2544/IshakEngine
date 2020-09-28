
#include <iostream>
#include <GLFW/glfw3.h>
#include "Window.h"

int main()
{

	using namespace Ishak;
	using namespace graphics;

	Window Window("Ishak", 800, 600);
	glClearColor(0.2f,0.3f,0.8f,1.0f);

	while (!Window.Closed())
	{

		std::cout << Window.GetWidth() << " , " << Window.GetHeight() << " ." << std::endl;

		Window.Clear();
		glBegin(GL_TRIANGLES);

		glVertex2f(-0.5f,-0.5f);
		glVertex2f(0.0f, 0.5f);
		glVertex2f(0.5f, -0.5f);

		glEnd();

		Window.Update();
	}



	return 0;
}