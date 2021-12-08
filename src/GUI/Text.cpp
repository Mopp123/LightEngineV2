
#include "Text.h"


namespace lightEngine
{
	namespace gui
	{
		Text::Text(const std::string& txt, float scale, int maxLineLength) :
			_str(txt), _fontScale(scale), _color(1,1,1,1), _maxLineLength(maxLineLength),
			Component(ComponentType::Text, sizeof(*this))
		{}

		Text::Text(const std::string& txt, float scale, mml::Vec4 color, int maxLineLength) :
			_str(txt), _fontScale(scale), _color(color), _maxLineLength(maxLineLength),
			Component(ComponentType::Text, sizeof(*this))
		{

		}

		Text::~Text()
		{}

	}
}