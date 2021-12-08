#pragma once

#include "glfw3.h"
#include "utils\maths\Maths.h"

namespace lightEngine
{

	class Window
	{
	private:

		GLFWwindow* _window = nullptr;

		static unsigned int s_width;
		static unsigned int s_height;

		static float s_aspectRatio;

		// orthographic projection matrix which' dimensions are : 
		// bottom left corner = 0,0 and top right corner = width,height
		static mml::Mat4 s_projectionMatrix;

	public:

		Window(const char* title, int width, int height, bool resizable, bool borderlessFullscreen, int swapInterval);
		~Window();

		void update();

		void close();
		bool isCloseRequested() const;

		static const unsigned int& get_width();
		static const unsigned int& get_height();
		static const float& get_aspect_ratio();
		static const mml::Mat4& get_projection_matrix();

		inline GLFWwindow* getGLFWwindow() { return _window; }
		
	private:

		void init(const char* title, bool resizable, bool borderlessFullscreen, int swapInterval);
	};
}