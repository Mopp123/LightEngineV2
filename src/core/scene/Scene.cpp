
#include "Scene.h"
#include "core\Application.h"

#include "core\Debug.h"


namespace lightEngine
{
	Scene::~Scene()
	{
		delete _activeCamera;

		for (Entity* e : _entities)
		{
			delete e;
			e = nullptr;
		}

		if (_directionalLight)
		{
			delete _directionalLight;
			_directionalLight = nullptr;
		}
	}

	void Scene::setup()
	{
		// Create default camera
		_activeCamera = new Camera(
			mml::create_projection_matrix__perspective(Window::get_aspect_ratio(), 70.0f, 0.1f, 1000.0f),
			mml::Vec3(0, 0, 0), 
			mml::Quat({ 0,1,0 }, 0),
			CameraControlType::Free
		);
	}

	template<typename T>
	static void extract_renderable_container_from_entity(const std::vector<T>& entityContainer, std::vector<T>& sceneRenderableContainer)
	{
		for (T renderable : entityContainer)
			sceneRenderableContainer.push_back(renderable);
	}

	void Scene::addToScene(Entity* entity)
	{
		_entities.push_back(entity);

		// extract all renderable components from the entity into separate containers
		extract_renderable_container_from_entity<gui::GUIImage*>(entity->getRenderableComponents_guiImages(), _renderables_guiImage);
		extract_renderable_container_from_entity<gui::Text*>(entity->getRenderableComponents_text(), _renderables_text);
		extract_renderable_container_from_entity<graphics::Renderable3D*>(entity->getRenderableComponents_3D(), _renderables_3D);
		extract_renderable_container_from_entity<graphics::SkinnedRenderable3D*>(entity->getRenderableComponents_skinned_3D(), _renderables_skinned_3D);
		extract_renderable_container_from_entity<graphics::TerrainRenderable*>(entity->getRenderableComponents_terrain(), _renderables_terrain);
	}



	// Fucking inefficient and gay!
	void Scene::handleEntities()
	{
		_activeCamera->update();

		for (Entity* e : _entities)
		{
			if (e->isActive())
			{
				// Update the entity
				e->update();
				e->getTransform().update();
				e->getTransform().updateTransformationMatrix();
			}
		}
	}

	void Scene::submitRenderables()
	{
		// Submit all renderables
		submitRenderableContainer(_renderables_guiImage);
		submitRenderableContainer(_renderables_text);
		submitRenderableContainer(_renderables_3D);
		submitRenderableContainer(_renderables_skinned_3D);
		submitRenderableContainer(_renderables_terrain);
	}


	template<typename T>
	void Scene::submitRenderableContainer(std::vector<T>& container) const
	{
		graphics::Graphics* applicationGraphics = Application::get_graphics();

		for (const auto* renderable : container)
		{
			if (renderable->getEntity()->isActive() && renderable->isActive())
				applicationGraphics->submit(renderable);
		}
	}
}