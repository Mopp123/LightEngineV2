#pragma once

#include "entities\Component.h"
#include "graphics\graphicsResources\Texture.h"
#include "utils\maths\Maths.h"

namespace lightEngine
{
	namespace gui
	{
		class GUIImage : public Component
		{
		private:
			graphics::Texture* _texture = nullptr; // *NOTE The GUIImage doesn't own its' texture, since textures are "resources"
			mml::Vec4 _color;

		public:
			GUIImage();
			GUIImage(graphics::Texture* textureToUse);

			inline void setColor(const mml::Vec4& color) { _color = color; }

			inline const graphics::Texture* getTexture() const { return _texture; }
			inline const mml::Vec4& getColor() const { return _color; }

		};
	}
}