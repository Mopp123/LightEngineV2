
#include <glfw3.h>
#include "FreeCameraEvent.h"
#include "utils\Time.h"

namespace lightEngine
{

	FreeCameraEvent::FreeCameraEvent(Camera* camera) : 
		InputEvent(INPUT_EVENT_TYPE__KEY | INPUT_EVENT_TYPE__MOUSE_BUTTON),
		_camera(camera)
	{
		addToInputHandler();
	}

	void FreeCameraEvent::keyCallback(int key, int scancode, int action, int mods) 
	{
		if (key == GLFW_KEY_W)
			action != GLFW_RELEASE ? addToMoveDirBit(CAMERA__MOVE_DIR__FORWARD) : removeFromMoveDirBit(CAMERA__MOVE_DIR__FORWARD);
		else if (key == GLFW_KEY_S)
			action != GLFW_RELEASE ? addToMoveDirBit(CAMERA__MOVE_DIR__BACKWARD) : removeFromMoveDirBit(CAMERA__MOVE_DIR__BACKWARD);

		if (key == GLFW_KEY_A)
			action != GLFW_RELEASE ? addToMoveDirBit(CAMERA__MOVE_DIR__LEFT) : removeFromMoveDirBit(CAMERA__MOVE_DIR__LEFT);
		else if (key == GLFW_KEY_D)
			action != GLFW_RELEASE ? addToMoveDirBit(CAMERA__MOVE_DIR__RIGHT) : removeFromMoveDirBit(CAMERA__MOVE_DIR__RIGHT);

		if (key == GLFW_KEY_SPACE)
			action != GLFW_RELEASE ? addToMoveDirBit(CAMERA__MOVE_DIR__UP) : removeFromMoveDirBit(CAMERA__MOVE_DIR__UP);
		else if (key == GLFW_KEY_LEFT_CONTROL)
			action != GLFW_RELEASE ? addToMoveDirBit(CAMERA__MOVE_DIR__DOWN) : removeFromMoveDirBit(CAMERA__MOVE_DIR__DOWN);
	}

	void FreeCameraEvent::cursorPosCallback(double x, double y) 
	{
	}

	void FreeCameraEvent::mouseButtonCallback(int button, int action, int mods) 
	{
		_enableRotating = action != GLFW_RELEASE;
	}

	void FreeCameraEvent::scrollCallback(double xOffset, double yOffset) {}



	void FreeCameraEvent::addToMoveDirBit(unsigned char bitmask)
	{
		_moveToDirections |= bitmask;
	}

	void FreeCameraEvent::removeFromMoveDirBit(unsigned char bitmask)
	{
		// If the "bit existed" -> remove it
		if (_moveToDirections & bitmask) _moveToDirections ^= bitmask;
	}

}