
#include "InputEvent.h"
#include "core\Application.h"

namespace lightEngine
{
	InputEvent::InputEvent(const unsigned char type) : 
		_type(type)
	{}
	
	InputEvent::~InputEvent()
	{}

	void InputEvent::addToInputHandler()
	{
		InputHandler* appIh = Application::get_input_handler();
		if (appIh)
			appIh->addEvent(this);
	}


	void InputEvent::keyCallback(int key, int scancode, int action, int mods)
	{}
	void InputEvent::cursorPosCallback(double x, double y)
	{}
	void InputEvent::mouseButtonCallback(int button, int action, int mods)
	{}
	void InputEvent::scrollCallback(double xOffset, double yOffset)
	{}
}