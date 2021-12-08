
#include "Light.h"

namespace lightEngine
{
	namespace graphics
	{
		static ComponentType convert_light_type_to_component(LightType type)
		{
			switch (type)
			{
			case LightType::PointLight :		return ComponentType::PointLight;
			case LightType::DirectionalLight:	return ComponentType::DirectionalLight;
			case LightType::SpotLight:			return ComponentType::SpotLight;
			default:
				break;
			}

			return ComponentType::None;
		}

		Light::Light(const mml::Vec3& color, LightType type) : 
			Component(convert_light_type_to_component(type), sizeof(*this)),
			_color(color)
		{}

	}
}