

#include "Entity.h"
#include "graphics\graphicsComponents\SkinnedRenderable3D.h"

#include "core\Debug.h"

namespace lightEngine
{

	Entity::Entity()
	{}
	Entity::Entity(const Transform& transform) : 
		_transform(transform)
	{}
	Entity::Entity(const std::string& name) : 
		_name(name)
	{}
	Entity::Entity(const std::string& name, const std::string& tag) : 
		_name(name), _tag(tag)
	{}
	Entity::Entity(const std::string& name, const std::string& tag, const std::string& layer) : 
		_name(name), _tag(tag), _layer(layer)
	{}

	Entity::~Entity()
	{
		for (Component* c : _components)
			delete c;
	}

	void Entity::addChild(Entity* entity)
	{
		_children.push_back(entity);
		entity->getTransform().setParent(&_transform);
		entity->_parent = this;
	}

	// Attempts to cast a component into a specific type. If the casting is successful -> stores the component into "container"
	template<typename T>
	static void attempt_component_storing(Component* component, std::vector<T>& container)
	{
		T castedComponent = dynamic_cast<T>(component);
		if (castedComponent)
			container.push_back(castedComponent);
	}

	void Entity::addComponent(Component* component)
	{
		component->_entity = this;
		_components.push_back(component);

		// This is quite dumb -> fix later
		switch (component->getType())
		{
		case ComponentType::GUIImage:				attempt_component_storing<gui::GUIImage*>(component, _components_guiImage);
			break;
		case ComponentType::Text:					attempt_component_storing<gui::Text*>(component, _components_text);
			break;
		case ComponentType::Renderable3D:			attempt_component_storing<graphics::Renderable3D*>(component, _components_renderable3D);
			break;
		case ComponentType::RenderableSkinned3D:	attempt_component_storing<graphics::SkinnedRenderable3D*>(component, _components_skinnedRenderable3D);
			break;
		case ComponentType::RenderableTerrain:		attempt_component_storing<graphics::TerrainRenderable*>(component, _components_terrainRenderable);
			break;
		default:
			break;
		}
	}


	template<typename T>
	std::vector<T*> Entity::findComponents()
	{
		std::vector<T*> foundComponents;
		foundComponents.reserve(_components.size()); // max size is the count of components..

		for (Component* c : _components)
		{
			T* t = dynamic_cast<T*>(c);
			if (t)
				foundComponents.emplace_back(t);
		}

		return foundComponents;
	}
}