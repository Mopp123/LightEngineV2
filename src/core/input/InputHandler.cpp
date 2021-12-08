
#include "InputHandler.h"
#include "core\Application.h"
#include "core\Debug.h"
#include "glfw3.h"

namespace lightEngine
{
	InputHandler* InputHandler::s_windowUserPointer = nullptr;

	double InputHandler::s_mouseX = 0.0;
	double InputHandler::s_mouseY = 0.0;
	double InputHandler::s_previousMouseX = 0.0;
	double InputHandler::s_previousMouseY = 0.0;

	float InputHandler::s_mouseDX = 0.0f;
	float InputHandler::s_mouseDY = 0.0f;


	void key_callback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods) 
	{
		for (std::pair<InputEvent*, void(InputEvent::*)(int, int, int, int)>& e : InputHandler::s_windowUserPointer->_keyEvents)
		{
			InputEvent* caller = e.first;
			void(InputEvent::* func)(int, int, int, int) = e.second;
			(caller->*func)(key, scancode, action, mods);
		}
	}

	void cursor_pos_callback(GLFWwindow* glfwWindow, double x, double y) 
	{
		InputHandler::s_mouseX = x;
		InputHandler::s_mouseY = y;

		for (std::pair<InputEvent*, void(InputEvent::*)(double, double)>& e : InputHandler::s_windowUserPointer->_cursorPosEvents)
		{
			InputEvent* caller = e.first;
			void(InputEvent::* func)(double, double) = e.second;
			(caller->*func)(x, y);
		}
	}
	void mouse_button_callback(GLFWwindow* glfwWindow, int button, int action, int mods) 
	{
		for (std::pair<InputEvent*, void(InputEvent::*)(int, int, int)>& e : InputHandler::s_windowUserPointer->_mouseButtonEvents)
		{
			InputEvent* caller = e.first;
			void(InputEvent::* func)(int, int, int) = e.second;
			(caller->*func)(button, action, mods);
		}
	}
	void scroll_callback(GLFWwindow* glfwWindow, double xOffset, double yOffset) 
	{
		for (std::pair<InputEvent*, void(InputEvent::*)(double, double)>& e : InputHandler::s_windowUserPointer->_scrollEvents)
		{
			InputEvent* caller = e.first;
			void(InputEvent::* func)(double, double) = e.second;
			(caller->*func)(xOffset, yOffset);
		}
	}


	InputHandler::InputHandler(GLFWwindow* glfwWindow)
	{
		init(glfwWindow);
	}

	void InputHandler::init(GLFWwindow* glfwWindow)
	{
		s_windowUserPointer = this;
		glfwSetWindowUserPointer(glfwWindow, s_windowUserPointer);

		glfwSetKeyCallback(glfwWindow, key_callback);
		glfwSetCursorPosCallback(glfwWindow, cursor_pos_callback);
		glfwSetMouseButtonCallback(glfwWindow, mouse_button_callback);
		glfwSetScrollCallback(glfwWindow, scroll_callback);
	}


	// This needs to be called at the end of each frame, so that there can be user inputting..
	void InputHandler::update()
	{
		s_previousMouseX = s_mouseX;
		s_previousMouseY = s_mouseY;

		glfwPollEvents();

		s_mouseDX = s_mouseX - s_previousMouseX;
		s_mouseDY = s_mouseY - s_previousMouseY;
	}

	void InputHandler::setMouseLock(bool arg)
	{
		_isMouseLocked = arg;
		glfwSetInputMode(Application::get_window()->getGLFWwindow(), GLFW_CURSOR, (int)_isMouseLocked);
	}

	void InputHandler::addEvent(InputEvent* inputEvent)
	{
		const unsigned char& type = inputEvent->getType();

		if (type & INPUT_EVENT_TYPE__KEY)				_keyEvents.push_back(std::make_pair(inputEvent, &InputEvent::keyCallback));
		if (type & INPUT_EVENT_TYPE__CURSOR_POSITION)	_cursorPosEvents.push_back(std::make_pair(inputEvent, &InputEvent::cursorPosCallback));
		if (type & INPUT_EVENT_TYPE__MOUSE_BUTTON)		_mouseButtonEvents.push_back(std::make_pair(inputEvent, &InputEvent::mouseButtonCallback));
		if (type & INPUT_EVENT_TYPE__SCROLL)			_scrollEvents.push_back(std::make_pair(inputEvent, &InputEvent::scrollCallback));
	}
	
	void InputHandler::clearAllEvents()
	{
		_keyEvents.clear();
		_cursorPosEvents.clear();
		_mouseButtonEvents.clear();
		_scrollEvents.clear();

		Debug::log("All input events cleared!");
	}



	double InputHandler::get_mouse_x() { return s_mouseX; }
	double InputHandler::get_mouse_y() { return s_mouseY; }

	float InputHandler::get_mouse_dx() { return s_mouseDX; }
	float InputHandler::get_mouse_dy() { return s_mouseDY; }
}