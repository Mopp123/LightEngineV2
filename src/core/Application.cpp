
#include "Application.h"
#include "Debug.h"


namespace lightEngine
{
	bool Application::s_isRunning = true;

	Window* Application::_applicationWindow = nullptr;
	InputHandler* Application::_applicationInputHandler = nullptr;
	SceneManager* Application::_applicationSceneManager = nullptr;
	ResourceManager* Application::_applicationResourceManager = nullptr;
	graphics::Graphics* Application::_applicationGraphics = nullptr;

	Application::Application(
		const char* title, int windowWidth, int windowHeight, bool windowResizable, bool borderlessFullscreen, int swapInterval,
		Scene* startupScene
	) :
		_window(title, windowWidth, windowHeight, windowResizable, borderlessFullscreen, swapInterval),
		_inputHandler(_window.getGLFWwindow()),
		_graphics(graphics::GraphicsAPI::OpenGL)
	{
		_applicationWindow = &_window;
		_applicationInputHandler = &_inputHandler;
		_applicationSceneManager = &_sceneManager;
		_applicationResourceManager = &_resourceManager;
		_applicationGraphics = &_graphics;

		_sceneManager.prepareSceneSwitch(startupScene);
	}

	Application::~Application()
	{
		ResourceManager::free_dynamic_resources();
		ResourceManager::free_static_resources();
	}


	void Application::update()
	{

		while (s_isRunning && !_window.isCloseRequested())
		{
			// First thing in the frame has to be this, so our delta time and fps are calculated correctly
			_time.beginFrame();

			_window.update();
			_inputHandler.update();
			_sceneManager.updateCurrentScene();

			_graphics.render(_sceneManager.getCurrentScene());

			// Always the last thing in the frame is to detect, do we want _sceneManager to change our current scene
			_sceneManager.detectSceneSwitch();

			// Last thing in the frame has to be this, so our delta time and fps are calculated correctly
			_time.endFrame();
		}
		
		// Destroy the last open scene..
		_sceneManager.destroyCurrentScene();

		// Quite dumb, but just in case..
		if(!_window.isCloseRequested())
			_window.close();

		glfwTerminate();
	}


	Window * const Application::get_window()
	{
		return _applicationWindow;
	}
	InputHandler * const Application::get_input_handler()
	{
		return _applicationInputHandler;
	}
	SceneManager * const Application::get_scene_manager()
	{
		return _applicationSceneManager;
	}
	ResourceManager * const Application::get_resource_manager()
	{
		return _applicationResourceManager;
	}
	graphics::Graphics * const Application::get_graphics()
	{
		return _applicationGraphics;
	}




	void Application::exit()
	{
		s_isRunning = false;
	}
}