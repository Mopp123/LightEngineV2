#pragma once

#include "Window.h"
#include "input\InputHandler.h"
#include "scene\SceneManager.h"
#include "resources\ResourceManager.h"
#include "graphics\Graphics.h"

#include "utils\Time.h"

/*

How to use : 

Create instance of Application.
call update().

*/

namespace lightEngine
{

	class Application
	{
	private:

		static bool s_isRunning;

		Window _window;
		InputHandler _inputHandler;
		SceneManager _sceneManager;
		ResourceManager _resourceManager;
		graphics::Graphics _graphics;
		
		Time _time;

		// So we can get access to these from anywhere within the program..
		static Window* _applicationWindow;
		static InputHandler* _applicationInputHandler;
		static SceneManager* _applicationSceneManager;
		static ResourceManager* _applicationResourceManager;
		static graphics::Graphics* _applicationGraphics;

	public:

		Application(
			const char* title, int windowWidth, int windowHeight, bool windowResizable, bool borderlessFullscreen, int swapInterval,
			Scene* startupScene
		);

		~Application();

		void update();

		static Window * const get_window();
		static InputHandler * const get_input_handler();
		static SceneManager * const get_scene_manager();
		static ResourceManager * const get_resource_manager();
		static graphics::Graphics * const get_graphics();

		static void exit();
	};

}