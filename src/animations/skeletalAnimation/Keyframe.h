#pragma once

#include "utils\maths\Maths.h"

namespace lightEngine
{
	namespace animation
	{
		class Keyframe
		{
		private:

			mml::Vec3 _position;
			mml::Quat _rotation;
			mml::Vec3 _scale;

		public:

			Keyframe(const mml::Vec3& pos, const mml::Quat& rot, const mml::Vec3& scale) :
				_position(pos), _rotation(rot), _scale(scale)
			{}

			Keyframe(const Keyframe& keyframe) :
				_position(keyframe._position), _rotation(keyframe._rotation), _scale(keyframe._scale)
			{}


			inline const mml::Vec3& getPosition() const { return _position; }
			inline const mml::Quat& getRotation() const { return _rotation; }
			inline const mml::Vec3& getScale() const { return _scale; }

		};
	}
}