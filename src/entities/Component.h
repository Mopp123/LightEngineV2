#pragma once

#include <string>

namespace lightEngine
{

	class Entity;

	enum class ComponentType
	{
		None = 0,
		Renderable2D,
		Renderable3D,
		RenderableSkinned3D,
		RenderableTerrain,
		Text,
		GUIImage,
		PointLight,
		DirectionalLight,
		SpotLight
	};


	class Component
	{
	protected:
		ComponentType _type;
		size_t _size;
		Entity* _entity;
		
		bool _isActive = true;

		friend class Entity;

	public:

		// You need to give the size of the object that inherits from this Component class.
		Component(ComponentType type, size_t size);
		virtual ~Component() {}

		inline void setActive(bool arg) { _isActive = arg; }

		inline const size_t getSize() const { return _size; }
		inline const ComponentType& getType() const { return _type; }
		inline Entity* getEntity() { return _entity; }
		inline const Entity * const getEntity() const { return _entity; }

		inline const bool isActive() const { return _isActive; }

		static std::string get_component_type_as_string(const ComponentType& type);
		static std::string get_component_type_as_string(const Component * const component);

	};
}