#pragma once

#include "entities\Component.h"
#include "utils\maths\Maths.h"

namespace lightEngine
{
	namespace graphics
	{

		enum class LightType
		{
			None = 0,
			PointLight,
			DirectionalLight,
			SpotLight
		};

		class Light : public Component
		{
		protected:

			mml::Vec3 _color;

		public:

			Light(const mml::Vec3& color, LightType type);

			inline void setColor(const mml::Vec3& color) { _color = color; }
			inline const mml::Vec3& getColor() const { return _color; }
		};
	}
}