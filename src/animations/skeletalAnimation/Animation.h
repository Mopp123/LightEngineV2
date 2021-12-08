#pragma once

#include "Keyframe.h"
#include <vector>

namespace lightEngine
{
	namespace animation
	{

		class Skeleton;


		class Animation
		{
		private:

			float _speed = 1.0f;
			float _interpolationAmount = 0.0f;

			unsigned int _currentKeyframeIndex = 0;
			unsigned int _nextKeyframeIndex = 0;

			unsigned int _startKeyframe = 0;
			unsigned int _endKeyframe = 0;

			bool _isPlaying = false;
			bool _hasEnded = true;

			friend class Skeleton;

		public:

			Animation(unsigned int startKeyframe, unsigned int endKeyframe, float speed);

			void loop();

			// Plays this animation once.
			// To be able to play this animation again, you'll need to 
			// call "setCurrentFrame(unsigned int frameIndex)" or "reset()" manually!
			void play();
			// Sets current anim frame to "keyframeIndex"
			void rewind(unsigned int keyframeIndex);
			// Resets the whole animation
			void reset();

			inline bool isPlaying() const { return _isPlaying; }
			inline bool hasEnded() const { return _hasEnded; }
		};
	}
}