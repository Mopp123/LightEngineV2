
#include "DirectionalLight.h"

namespace lightEngine
{
	namespace graphics
	{
		DirectionalLight::DirectionalLight(const mml::Vec3& direction, const mml::Vec3& color) : 
			Light(color, LightType::DirectionalLight),
			_direction(direction)
		{
		}
	}
}