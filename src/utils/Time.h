#pragma once

namespace lightEngine
{
	class Time
	{
	private:
		int _frames = 0;

		double _fps_startTime = 0;
		double _delta_startTime = 0;

		bool _firstFrame = true;

	public:
		static double FPS;
		static double DELTA;

	public:

		Time();

		void beginFrame();
		void endFrame();

	};
}