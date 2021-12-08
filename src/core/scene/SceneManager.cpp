
#include "SceneManager.h"
#include "core\Application.h"

#include "core\Debug.h"

#include <thread>

namespace lightEngine
{
	SceneManager::SceneManager()
	{}
	SceneManager::~SceneManager()
	{}

	void SceneManager::updateCurrentScene()
	{
		if (_currentScene)
		{
			_currentScene->update();
			_currentScene->handleEntities();
			_currentScene->submitRenderables();
		}
	}

	// Prepares new scene to be switched into
	// (*This doesn't immediately change the scene)
	void SceneManager::prepareSceneSwitch(Scene* newScene)
	{
		if (_tempNextScene != nullptr)
			Debug::log(
				"WARNING: Attempted to change the scene, but _tempNextScene wasn't nullptr.\n"
				"This may be caused by calling 'prepareSceneSwitch(Scene*)' multiple times within a single frame."
				"You propably don't want to do that?"
			);

		_tempNextScene = newScene;
	}



	// Does the actual scene switching. 
	// Called at the end of each SceneManager::update()
	// If prepareSceneSwitch() has been called anywhere in the current frame, 
	// this destroys the current scene and makes the current scene be the new scene.
	void SceneManager::detectSceneSwitch()
	{
		if (_tempNextScene != nullptr)
		{
			Application::get_input_handler()->clearAllEvents();
			destroyCurrentScene();
			_currentScene = _tempNextScene;
			_tempNextScene = nullptr;

			_currentScene->setup();	// does all the default setup that is the same for each scen ("NOT user defined")
			_currentScene->init();	// "user defined"
		}
	}


	void SceneManager::destroyCurrentScene()
	{
		ResourceManager::free_dynamic_resources();
		delete _currentScene;
	}
}