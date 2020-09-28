#include "Window.h"
#include <iostream>

namespace Ishak {
	namespace graphics {

		void WindowResize(GLFWwindow* Window, int Width, int Height);


		Window::Window(const char* Title, int Width, int Height)
		{

			m_Title = Title;
			m_Height = Height;
			m_Width = Width;

			if (!Init())
			{

			glfwTerminate();

			}

		}

		Window::~Window()
		{

			glfwTerminate();

		}


		void Window::Clear() const
		{

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		}

		void Window::Update() 
		{

		glfwPollEvents();
		
		glfwSwapBuffers(m_Window);

		}

		bool Window::Closed() const
		{
			return glfwWindowShouldClose(m_Window) == 1;
		}

		


		bool Window::Init()
		{

			if (!glfwInit())
			{

				std::cout << "Failed to Initialize GLFW!" << std::endl;
				return false;

			}
			

			m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, NULL, NULL);

			if (!m_Window)
			{

				std::cout << "Failed to create the Window" << std::endl;
				return false;

			}

			glfwMakeContextCurrent(m_Window);
			glfwSetWindowSizeCallback(m_Window,WindowResize);
			

			return true;
		}

		void WindowResize(GLFWwindow* Window, int Width, int Height)
		{
			glViewport(0, 0, Width, Height);

			std::cout << "Resizeddd !! " << std::endl;
		}

	}
}