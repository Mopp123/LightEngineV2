
#include "Component.h"
#include "Entity.h"

namespace lightEngine
{

	// You need to give the size of the object that inherits from this Component class.
		// You also need to specify the this component's owner entity
	Component::Component(ComponentType type, size_t size) :
		_size(size), _type(type)
	{
	}


	std::string Component::get_component_type_as_string(const ComponentType& type)
	{
		switch (type)
		{
		case ComponentType::None:					return "None";
		case ComponentType::GUIImage:				return "GUIImage";
		case ComponentType::Text:					return "Text";
		case ComponentType::Renderable2D:			return "Renderable2D";
		case ComponentType::Renderable3D:			return "Renderable3D";
		case ComponentType::RenderableSkinned3D:	return "RenderableSkinned3D";

		default:
			break;
		}

		return "Error";
	}

	std::string Component::get_component_type_as_string(const Component * const component)
	{
		return get_component_type_as_string(component->getType());
	}

}