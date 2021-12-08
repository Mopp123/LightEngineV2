#pragma once

#include <vector>
#include "InputEvent.h"
#include <glfw3.h>


namespace lightEngine
{
	class InputHandler
	{
	private:

		static InputHandler* s_windowUserPointer;
		
		// pair of caller and function..
		std::vector<std::pair<InputEvent*, void(InputEvent::*)(int, int, int, int)>>	_keyEvents; 
		std::vector<std::pair<InputEvent*, void(InputEvent::*)(double, double)>>		_cursorPosEvents;
		std::vector<std::pair<InputEvent*, void(InputEvent::*)(int, int, int)>>			_mouseButtonEvents;
		std::vector<std::pair<InputEvent*, void(InputEvent::*)(double, double)>>		_scrollEvents;

		bool _isMouseLocked = false;

		static double s_mouseX;
		static double s_mouseY;
		static double s_previousMouseX;
		static double s_previousMouseY;
		static float s_mouseDX;
		static float s_mouseDY;


	public:

		InputHandler(GLFWwindow* glfwWindow);

		// This needs to be called at the end of each frame, so that there can be user inputting..
		void update();

		void setMouseLock(bool arg);

		friend void key_callback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods);
		friend void cursor_pos_callback(GLFWwindow* glfwWindow, double x, double y);
		friend void mouse_button_callback(GLFWwindow* glfwWindow, int button, int action, int mods);
		friend void scroll_callback(GLFWwindow* glfwWindow, double xOffset, double yOffset);

		void addEvent(InputEvent* inputEvent);
		void clearAllEvents();


		static double get_mouse_x();
		static double get_mouse_y();

		static float get_mouse_dx();
		static float get_mouse_dy();

	private:

		void init(GLFWwindow* glfwWindow);
	};
}