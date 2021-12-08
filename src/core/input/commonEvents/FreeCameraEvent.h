#pragma once

#include "entities\Camera.h"
#include "core\input\InputEvent.h"

#define FREE_CAMERA_EVENT__DEFAULT_SETTINGS__MOV_SPEED 50.0f
#define FREE_CAMERA_EVENT__DEFAULT_SETTINGS__ROT_SPEED 70.0f

namespace lightEngine
{
	class FreeCameraEvent : public InputEvent
	{
	private:

		Camera* _camera;

		unsigned char _moveToDirections = 0;

		float _pitch = 0.0f;
		float _yaw = 0.0f;

		bool _enableRotating = false;

		friend class Camera;

	public:

		FreeCameraEvent(Camera* camera);

		virtual void keyCallback(int key, int scancode, int action, int mods) override;
		virtual void cursorPosCallback(double x, double y) override;
		virtual void mouseButtonCallback(int button, int action, int mods) override;
		virtual void scrollCallback(double xOffset, double yOffset) override;

	private:

		void addToMoveDirBit(unsigned char bitmask);
		void removeFromMoveDirBit(unsigned char bitmask);

	};
}