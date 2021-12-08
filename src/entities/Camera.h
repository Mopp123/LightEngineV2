#pragma once

#include "Entity.h"
#include "core\input\InputEvent.h"

#define CAMERA__MOVE_DIR__FORWARD	0b000001
#define CAMERA__MOVE_DIR__BACKWARD	0b000010
#define CAMERA__MOVE_DIR__LEFT		0b000100
#define CAMERA__MOVE_DIR__RIGHT		0b001000
#define CAMERA__MOVE_DIR__UP		0b010000
#define CAMERA__MOVE_DIR__DOWN		0b100000

namespace lightEngine
{
	enum class CameraControlType
	{
		Custom, Free, Editor
	};

	class Camera : public Entity
	{
	private:

		mml::Vec3 prevPos;
		mml::Quat prevRot;

		mml::Mat4 _projectionMatrix;

		CameraControlType _controlType = CameraControlType::Custom;
		InputEvent* _controlInputEvent = nullptr;

	public:

		Camera(
			const mml::Mat4& projectionMatrix, 
			const mml::Vec3& pos, 
			const mml::Quat& rot, 
			CameraControlType controlType = CameraControlType::Custom
		);
		~Camera();

		virtual void update() override;

		void setControlType(CameraControlType type);
		
		const mml::Mat4& getViewMatrix() const;
		inline const mml::Mat4& getProjectionMatrix() const { return _projectionMatrix; };

	private:

		void updateFreeControls();
		void move(const mml::Vec3& direction, const float speed);
	};
}