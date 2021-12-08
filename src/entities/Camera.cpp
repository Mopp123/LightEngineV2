
#include "Camera.h"
#include "utils\Time.h"
#include "utils\maths\Maths.h"

#include "core\input\commonEvents\FreeCameraEvent.h"

#include "core\Application.h"
#include "core\Debug.h"

namespace lightEngine
{


	Camera::Camera(
		const mml::Mat4& projectionMatrix, 
		const mml::Vec3& pos, 
		const mml::Quat& rot, 
		CameraControlType controlType
	) :
		Entity(Transform(pos, rot, mml::Vec3(1,1,1))),
		_projectionMatrix(projectionMatrix),
		_controlType(controlType)
	{
		setControlType(_controlType);
	}

	Camera::~Camera()
	{
		if (_controlInputEvent) // *NOTE This may not be nullptr even if it was deleted..
			delete _controlInputEvent;
	}

	void Camera::update()
	{
		const mml::Vec3& pos = _transform.getGlobalPosition();
		const mml::Quat& rot = _transform.getGlobalRotation();

		// If camera hadn't move from previous update -> dont update its' "view matrix"
		mml::update_view_matrix(_transform.getTransformationMatrix(), pos, rot);

		prevPos = pos;
		prevRot = rot;



		switch (_controlType)
		{
		case CameraControlType::Free : updateFreeControls();
			break;
		default:
			break;
		}

		_transform.update();
	}


	void Camera::setControlType(CameraControlType type)
	{
		_controlType = type;

		// delete previous control input event, if there was one..
		if (_controlInputEvent)
		{
			delete _controlInputEvent;
			_controlInputEvent = nullptr;
		}

		switch (type)
		{
		case CameraControlType::Free: _controlInputEvent = new FreeCameraEvent(this);
			break;
		default:
			break;
		}
	}

	const mml::Mat4& Camera::getViewMatrix() const
	{
		return _transform.getTransformationMatrix();
	}


	void Camera::updateFreeControls()
	{
#ifdef LIGHT_ENGINE__DEBUG_MODE__FULL
		if (!_controlInputEvent)
		{
			Debug::log("ERROR: Location: Camera::updateFreeControls()\nTried to update free controls for camera, but _controlInputEvent was nullptr!");
			Application::exit();
			return;
		}
#endif


		FreeCameraEvent* ev = (FreeCameraEvent*)_controlInputEvent; // quite dangerous..

		float movementSpeed = 20.0f;
		float rotationSpeed = 40.0f;

		// Rotating..
		if (ev->_enableRotating)
		{
			float rotSpeedMod = 0.5f;
			ev->_pitch -= InputHandler::get_mouse_dy() * rotSpeedMod * Time::DELTA;
			ev->_yaw -= InputHandler::get_mouse_dx() * rotSpeedMod * Time::DELTA;

			mml::Quat pitch({ 1, 0, 0 }, ev->_pitch);
			mml::Quat yaw({ 0, 1, 0 }, ev->_yaw);

			mml::Quat totalRotation = yaw * pitch;
			totalRotation.normalize();

			_transform.setRotation(totalRotation);
		}

		const mml::Vec3 forw = _transform.forward();
		const mml::Vec3 left = _transform.left();
		const mml::Vec3 up = _transform.up();

		if (ev->_moveToDirections & CAMERA__MOVE_DIR__FORWARD)
			move(forw, movementSpeed);
		else if (ev->_moveToDirections & CAMERA__MOVE_DIR__BACKWARD)
			move(forw, -movementSpeed);

		if (ev->_moveToDirections & CAMERA__MOVE_DIR__LEFT)
			move(left, movementSpeed);
		else if (ev->_moveToDirections & CAMERA__MOVE_DIR__RIGHT)
			move(left, -movementSpeed);

		if (ev->_moveToDirections & CAMERA__MOVE_DIR__UP)
			move(up, movementSpeed);
		else if (ev->_moveToDirections & CAMERA__MOVE_DIR__DOWN)
			move(up, -movementSpeed);
			
	}

	void Camera::move(const mml::Vec3& direction, const float speed)
	{
		mml::Vec3 pos = _transform.getGlobalPosition();
		pos = pos + direction * speed * Time::DELTA;
		_transform.setPosition(pos);

		_transform.update();// May be quite rough to update this all the time..
	}
}