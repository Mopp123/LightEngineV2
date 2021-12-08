#pragma once

#include "Light.h"

namespace lightEngine
{
	namespace graphics
	{
		class DirectionalLight : public Light
		{
		private:

			mml::Vec3 _direction;

		public:

			DirectionalLight(const mml::Vec3& direction, const mml::Vec3& color);

			inline void setDirection(const mml::Vec3& direction) { _direction = direction; }
			inline const mml::Vec3& getDirection() const { return _direction; }

		};
	}
}