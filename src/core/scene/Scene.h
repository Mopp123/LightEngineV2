#pragma once

#include "entities\Entity.h"
#include "entities\Camera.h"
#include "graphics\graphicsComponents\lights\DirectionalLight.h"
#include <vector>

/*

	Scene:

	All resource allocation done by the scene happens in its' init() function.
	(Note that constructor doesn't allocate any scene's resources!)
*/

namespace lightEngine
{
	class SceneManager;

	class Scene
	{
	protected:

		Camera* _activeCamera = nullptr;
		std::vector<Entity*> _entities;

		// All renderable things of the scene are stored into their own containers
		//	-> This helps us to speed up submitting all these for rendering
		std::vector<graphics::Renderable3D*> _renderables_3D;
		std::vector<graphics::SkinnedRenderable3D*> _renderables_skinned_3D;
		std::vector<graphics::TerrainRenderable*> _renderables_terrain;

		std::vector<gui::GUIImage*> _renderables_guiImage;
		std::vector<gui::Text*> _renderables_text;

		graphics::DirectionalLight* _directionalLight;


		friend class SceneManager;

	public:

		Scene() {}
		virtual ~Scene();

		void setup(); // Happens before init
		virtual void init() {}
		virtual void update() {}

		inline const Camera* getActiveCamera() const { return _activeCamera; }

		inline const graphics::DirectionalLight* getDirectionalLight() const { return _directionalLight; }

		void addToScene(Entity* entity);

	private:

		void handleEntities();
		void submitRenderables();

		template<typename T>
		void submitRenderableContainer(std::vector<T>& container) const;
	};
}