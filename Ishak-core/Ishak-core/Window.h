#pragma once
#include <GLFW/glfw3.h>

namespace Ishak{ namespace graphics {


	class Window {

	private:

		const char* m_Title;
		int m_Width , m_Height;
		bool m_Closed = false;

		GLFWwindow* m_Window = nullptr;

	public:

		Window(const char* Title , int Width , int Height);

		~Window();

		bool Closed () const ;
		void Update() ;
		void Clear() const;

		inline int GetWidth() const {return m_Width;}

		inline int GetHeight() const {return m_Width;}


	private:

	bool Init();

	

};
}

}
