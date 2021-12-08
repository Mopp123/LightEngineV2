#pragma once

#include "entities\Component.h"
#include "utils\maths\Maths.h"
#include <string>

namespace lightEngine
{
	namespace gui
	{

		class Text : public Component
		{
		private:
			std::string _str;
			mml::Vec4 _color;

			float _fontScale;

			// -1 means this never changes line
			int _maxLineLength = -1;

		public:

			// * Scale is percentage of the size of the used font's character's actual scale.
			// For example : 
			// Current font's each character's bitmap has dimensions 64x64
			//	-> if we use scale of 0.5 it means that our actual scale of a character (in pixels) is 32
			Text(const std::string& txt, float scale, int maxLineLength = -1);
			Text(const std::string& txt, float scale, mml::Vec4 color, int maxLineLength = -1);
			virtual ~Text();
			
			inline void set(const std::string& text) { _str = text; }
			inline void setColor(const mml::Vec4& color) { _color = color; }
			inline void setFontSize(const float& size) { _fontScale = size; }
			inline void setMaxLineLength(const unsigned int length) { _maxLineLength = length; }

			inline const std::string& getString() const { return _str; }
			inline const mml::Vec4& getColor() const { return _color; }
			inline const float& getFontSize() const { return _fontScale; }
			inline const unsigned int& getMaxLineLength() const { return _maxLineLength; }
		};
	}
}