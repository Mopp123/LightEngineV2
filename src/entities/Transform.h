#pragma once

#include "utils\maths\Maths.h"

namespace lightEngine
{

	class Transform
	{
	private:

		mml::Vec3 _globalPosition;
		mml::Vec3 _localPosition;

		mml::Quat _globalRotation;
		mml::Quat _localRotation;

		mml::Vec3 _globalScale;
		mml::Vec3 _localScale;

		mml::Mat4 _transformationMatrix;

		Transform* _parent = nullptr;

	public:

		Transform();
		Transform(const mml::Vec3& pos, const mml::Quat& rot, const mml::Vec3& scale);

		// Updates global position, rotation and scale according to parent (If this transform has a parent)
		void update();

		// wtf u think it does?
		void updateTransformationMatrix();

		mml::Vec3 forward() const;
		mml::Vec3 up() const;
		mml::Vec3 left() const;

		void setPosition(const mml::Vec3& pos);
		void setRotation(const mml::Quat& rot);
		void setScale(const mml::Vec3& scale);

		void setParent(Transform* parent);

		mml::Vec3 getGlobalPosition() const;
		mml::Quat getGlobalRotation() const;
		mml::Vec3 getGlobalScale() const;

		inline const mml::Vec3& getLocalPosition() const { return _localPosition; }
		inline const mml::Quat& getLocalRotation() const { return _localRotation; }
		inline const mml::Vec3& getLocalScale() const { return _localScale; }

		inline mml::Mat4& getTransformationMatrix() { return _transformationMatrix; }
		inline const mml::Mat4& getTransformationMatrix() const { return _transformationMatrix; }

		inline Transform* getParent() const { return _parent; }
	};
}