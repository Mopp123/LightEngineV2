
#include "GUIImage.h"


namespace lightEngine
{
	namespace gui
	{
		GUIImage::GUIImage() :
			Component(ComponentType::GUIImage, sizeof(*this))
		{
		}

		GUIImage::GUIImage(graphics::Texture* textureToUse) :
			Component(ComponentType::GUIImage, sizeof(*this))
		{
			_texture = textureToUse;
		}

	}
}