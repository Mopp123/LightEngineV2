#pragma once

#include "Scene.h"


/*
	About scene changing:

	You can call "prepareSceneSwitch(Scene* newScene)" from anywhere within the program.
		-> This makes the scene to be switched at the end of the current frame.

	We delay scene switch to the end of the current frame, because this guarantees that
	nothing can go wrong in rendering or any other systems that may be in between the prepareSceneSwitch(Scene* newScene) 
	and the beginning of the next frame.

*/

namespace lightEngine
{
	class SceneManager
	{
	private:

		Scene* _currentScene = nullptr;
		Scene* _tempNextScene = nullptr;

		friend class Application;

	public:

		SceneManager();
		~SceneManager();

		void updateCurrentScene();

		// Prepares new scene to be switched into
		// (*This doesn't immediately change the scene)
		void prepareSceneSwitch(Scene* newScene);
		
		void destroyCurrentScene();

		inline Scene * const getCurrentScene() { return _currentScene; }


	private:

		// Does the actual scene switching. 
		// Called at the end of each SceneManager::update()
		// If prepareSceneSwitch() has been called anywhere in the current frame, 
		// this destroys the current scene and makes the current scene be the new scene.
		void detectSceneSwitch();

	};
}