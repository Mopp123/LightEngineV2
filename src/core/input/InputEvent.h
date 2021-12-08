#pragma once

#define INPUT_EVENT_TYPE__KEY				0x1
#define INPUT_EVENT_TYPE__MOUSE_BUTTON		0x2
#define INPUT_EVENT_TYPE__CURSOR_POSITION	0x4
#define INPUT_EVENT_TYPE__SCROLL			0x8

/*
#define INPUT_EVENT_TYPE__KEY				0b00000001
#define INPUT_EVENT_TYPE__MOUSE_BUTTON		0b00000010
#define INPUT_EVENT_TYPE__CURSOR_POSITION	0b00000100
#define INPUT_EVENT_TYPE__SCROLL			0b00001000
*/

namespace lightEngine
{
	class InputEvent
	{
	private:
		unsigned char _type = 0;
		
	public:

		InputEvent(const unsigned char type);
		virtual ~InputEvent();

		// Needs to be called so that this event can work..
		// This can be called in scene's init and later(until we switch scene).
		void addToInputHandler();
		
		// Reason, why these aren't pure virtual, is that we don't need to always write a definition 
		// for a callback if we aren't even going to do anything in that function
		virtual void keyCallback(int key, int scancode, int action, int mods);
		virtual void cursorPosCallback(double x, double y);
		virtual void mouseButtonCallback(int button, int action, int mods);
		virtual void scrollCallback(double xOffset, double yOffset);

		inline const unsigned char& getType() const { return _type; }
	};
}