
#include "Transform.h"
#include <stdio.h>

namespace lightEngine
{
	Transform::Transform()
	{
		_globalScale = mml::Vec3(1, 1, 1);
		_localScale = mml::Vec3(1, 1, 1);
	}
	Transform::Transform(const mml::Vec3& pos, const mml::Quat& rot, const mml::Vec3& scale) : 
		_localPosition(pos), _localRotation(rot), _localScale(scale)
	{
	}


	void Transform::update()
	{
		if (_parent)
		{
			mml::Quat parentRot = _parent->getGlobalRotation();
			mml::Vec3 parentPos = _parent->getGlobalPosition();

			// Rotation
			_globalRotation = parentRot * _localRotation;

			// Position
			parentRot.normalize(); // not sure is this necessary..
			mml::Quat q = parentRot * _localPosition * parentRot.conjugate();
			_globalPosition = mml::Vec3(parentPos.x + q.x, parentPos.y + q.y, parentPos.z + q.z);


			_globalScale = _parent->getGlobalScale() * _localScale;
		}
		else
		{
			_globalPosition = _localPosition;
			_globalRotation = _localRotation;
			_globalScale = _localScale;
		}
	}


	// wtf u think it does?
	void Transform::updateTransformationMatrix()
	{
		mml::update_transformation_matrix(_transformationMatrix, getGlobalPosition(), getGlobalRotation(), _localScale);
	}

	// *NOTE Somethings fucky here, where i found these (https://www.gamedev.net/forums/topic/56471-extracting-direction-vectors-from-quaternion/)
	// In these my systems, up seems to be left and forward up, ..or something...
	mml::Vec3 Transform::forward() const
	{
		mml::Vec3 forw;
		mml::Quat rotation = getGlobalRotation();

		forw.x = 2.0f * (rotation.x * rotation.z + rotation.w * rotation.y);
		forw.y = 2.0f * (rotation.y*rotation.z - rotation.w * rotation.x);
		forw.z = 1.0f - 2.0f * (rotation.x * rotation.x + rotation.y * rotation.y);
		forw.normalize();

		forw = forw * -1; // we want to invert this..

		return forw;
	}

	mml::Vec3 Transform::up() const
	{
		mml::Vec3 up;
		const mml::Quat& rotation = getGlobalRotation();

		up.x = 2.0f * (rotation.x*rotation.y + rotation.w * -rotation.z);
		up.y = 1.0f - 2.0f * (rotation.x*rotation.x + -rotation.z * -rotation.z);
		up.z = 2.0f * -(rotation.y*-rotation.z - rotation.w * rotation.x);
		up.normalize();

		return up;
	}

	mml::Vec3 Transform::left() const
	{
		mml::Vec3 left;
		const mml::Quat& rotation = getGlobalRotation();

		left.x = -(1 - 2 * (rotation.y*rotation.y + rotation.z * rotation.z));
		left.y = 2 * (rotation.x*rotation.y + rotation.w * rotation.z);
		left.z = -(2 * (rotation.x*rotation.z - rotation.w * rotation.y));
		left.normalize();

		return left;
	}



	void Transform::setPosition(const mml::Vec3& pos)
	{
		_localPosition = pos;
	}

	void Transform::setRotation(const mml::Quat& rot)
	{
		_localRotation = rot;
	}

	void Transform::setScale(const mml::Vec3& scale)
	{
		_localScale = scale;
	}

	void Transform::setParent(Transform* parent)
	{
		_parent = parent;
	}


	mml::Vec3 Transform::getGlobalPosition() const
	{
		return _globalPosition;
	}

	mml::Quat Transform::getGlobalRotation() const
	{
		return _globalRotation;
	}

	mml::Vec3 Transform::getGlobalScale() const
	{
		return _localScale;
	}
}