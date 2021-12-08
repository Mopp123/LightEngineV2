
#include "Animation.h"
#include "Skeleton.h"
#include "utils\Time.h"
#include "core\Debug.h"

namespace lightEngine
{
	namespace animation
	{
		Animation::Animation(unsigned int startKeyframe, unsigned int endKeyframe, float speed) :
			_startKeyframe(startKeyframe),
			_endKeyframe(endKeyframe),
			_speed(speed)
		{
			_currentKeyframeIndex = _startKeyframe;
			_nextKeyframeIndex = _currentKeyframeIndex + 1;
		}

		void Animation::loop()
		{
			_isPlaying = true;

			_interpolationAmount += _speed * Time::DELTA;

			if (_interpolationAmount >= 1.0f)
			{
				// If animation has still frames left -> continue
				if (_nextKeyframeIndex + 1 <= _endKeyframe)
				{
					_currentKeyframeIndex++;
					_nextKeyframeIndex++;
					_interpolationAmount = 0.0f;
				}
				else
				{
					// If we have played the whole animation -> rewind back to beginning
					reset();
				}
			}
		}

		// Plays this animation once.
		// To be able to play this animation again, you'll need to 
		// call "setCurrentFrame(unsigned int frameIndex)" or "reset()" manually!
		void Animation::play()
		{
			_isPlaying = true;
			_hasEnded = false;

			_interpolationAmount += _speed * Time::DELTA;

			if (_interpolationAmount >= 1.0f)
			{
				// If animation has still frames left -> continue
				if (_nextKeyframeIndex + 1 <= _endKeyframe)
				{
					_currentKeyframeIndex++;
					_nextKeyframeIndex++;
					_interpolationAmount = 0.0f;
				}
				else
				{
					// If we have played the whole animation -> stop
					_isPlaying = false;
					_hasEnded = true;
					return;
				}
			}
		}

		// Sets current anim frame to "keyframeIndex"
		void Animation::rewind(unsigned int keyframeIndex) 
		{
			_currentKeyframeIndex = keyframeIndex;
			_nextKeyframeIndex = _currentKeyframeIndex + 1;
			_interpolationAmount = 0.0f;
		}
		// Resets the whole animation
		void Animation::reset() 
		{
			rewind(_startKeyframe);
		}

	}
}