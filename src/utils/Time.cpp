
#include "Time.h"
#include <glfw3.h>
#include <stdio.h>

namespace lightEngine
{
	double Time::FPS = 0;
	double Time::DELTA = 0;

	Time::Time()
	{}

	void Time::beginFrame()
	{
		_frames++;
		_delta_startTime = glfwGetTime();
	}

	void Time::endFrame()
	{
		if (_frames > 10 && glfwGetTime() - _fps_startTime > 0.25)
		{
			FPS = (double)(_frames) / (glfwGetTime() - _fps_startTime);
			_frames = 0;
			_fps_startTime = glfwGetTime();
		}

		DELTA = glfwGetTime() - _delta_startTime;
	}
}