#pragma once

#include <vector>

#include "Component.h"
#include "Transform.h"

#include "graphics\graphicsResources\Mesh.h"
#include "GUI\GUIImage.h"
#include "GUI\Text.h"
#include "graphics\graphicsComponents\Renderable3D.h"
#include "graphics\graphicsComponents\TerrainRenderable.h"

namespace lightEngine
{

	namespace graphics
	{
		class SkinnedRenderable3D;
	}


	class Entity
	{
	protected:


		Entity* _parent = nullptr;
		Transform _transform;
		
		std::vector<Entity*>			_children;
		std::vector<Component*>			_components;
		
		// All renderable components are stored together in the entity for simplicity's sake
		std::vector<gui::GUIImage*>					_components_guiImage;
		std::vector<gui::Text*>						_components_text;
		std::vector<graphics::Renderable3D*>		_components_renderable3D;
		std::vector<graphics::SkinnedRenderable3D*>	_components_skinnedRenderable3D;
		std::vector<graphics::TerrainRenderable*>	_components_terrainRenderable;


		std::string _name = "New Entity";
		std::string _tag = "";
		std::string _layer = "";

		bool _isActive = true;

	public:

		Entity();
		Entity(const Transform& transform);

		Entity(const std::string& name);
		Entity(const std::string& name, const std::string& tag);
		Entity(const std::string& name, const std::string& tag, const std::string& layer);
		
		virtual ~Entity();

		void addChild(Entity* entity);
		void addComponent(Component* component);

		virtual void update() {}
		
		// Finds and returns all components of type T
		template<typename T>
		std::vector<T*> findComponents();

		const std::vector<gui::GUIImage*>& getRenderableComponents_guiImages()					const { return _components_guiImage; }
		const std::vector<gui::Text*>& getRenderableComponents_text()							const { return _components_text; }
		const std::vector<graphics::Renderable3D*>& getRenderableComponents_3D()				const { return _components_renderable3D; }
		const std::vector<graphics::SkinnedRenderable3D*>& getRenderableComponents_skinned_3D()	const { return _components_skinnedRenderable3D; }
		const std::vector<graphics::TerrainRenderable*>& getRenderableComponents_terrain()		const { return _components_terrainRenderable; }


		inline Entity* getParent() { return _parent; }
		inline Transform& getTransform() { return _transform; }
		inline const Transform& getTransform() const { return _transform; }

		inline void setActive(bool arg) { _isActive = arg; }
		inline std::vector<Component*> getAllComponents() const { return _components; }
		inline std::vector<Entity*> getAllChildren() { return _children; }
		
		inline const std::string& getName() const	{ return _name; }
		inline const std::string& getTag() const	{ return _tag; }
		inline const std::string& getLayer() const	{ return _layer; }
		
		inline const bool isActive() const { return _isActive; }

	};
}