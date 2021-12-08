
#include <glew.h>
#include "Window.h"
#include "Debug.h"

namespace lightEngine
{
	unsigned int Window::s_width = 800;
	unsigned int Window::s_height= 600;
	float Window::s_aspectRatio = 800.0f / 600.0f;
	mml::Mat4 Window::s_projectionMatrix;

	Window::Window(const char* title, int width, int height, bool resizable, bool borderlessFullscreen, int swapInterval)
	{
		s_width = width;
		s_height = height;
		s_aspectRatio = (float)s_width / (float)s_height;

		init(title, resizable, borderlessFullscreen, swapInterval);

		s_projectionMatrix = mml::create_projection_matrix__orthographic(0, s_width, s_height, 0, 1, -1);
	}
	
	Window::~Window()
	{
	}

	void Window::init(const char* title, bool resizable, bool borderlessFullscreen, int swapInterval)
	{
		if (!glfwInit())
		{
			Debug::log("ERROR: Failed to init GLFW!\nNow exiting...");
			return;
		}
		else
		{
			Debug::log("GLFW init was successful!");
		}

		GLFWmonitor* primaryMonitor = borderlessFullscreen ? glfwGetPrimaryMonitor() : NULL;

		glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE); // kind of pointless..

		_window = glfwCreateWindow(s_width, s_height, title, primaryMonitor, NULL);
		if (_window == nullptr)
		{
			Debug::log("ERROR: Failed to create GLFW window!\nNow exiting...");
			glfwTerminate(); // if glfwInit() has been called, this should be done...
			return;
		}
		else
		{
			Debug::log("GLFW window created successfully!");
		}

		glfwMakeContextCurrent(_window);
		glfwSwapInterval(swapInterval);
		glfwShowWindow(_window);

		if (glewInit() != GLEW_OK)
		{
			Debug::log("ERROR: Failed to init GLEW!");
			return;
		}
		else
		{
			Debug::log("GLEW init successful!");
		}
	}


	void Window::close()
	{
		glfwSetWindowShouldClose(_window, GLFW_TRUE);
	}

	bool Window::isCloseRequested() const
	{
		return (bool)glfwWindowShouldClose(_window);
	}

	void Window::update()
	{
		glfwSwapBuffers(_window);
	}

	const unsigned int& Window::get_width()		{ return s_width; }
	const unsigned int& Window::get_height()	{ return s_height; }
	const float& Window::get_aspect_ratio()		{ return s_aspectRatio; }
	const mml::Mat4& Window::get_projection_matrix() { return s_projectionMatrix; }
}