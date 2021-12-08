
#include "Texture.h"
#include "graphics\Graphics.h"
#include "graphics\opengl\openglGraphicsResources\OpenglTexture.h"


#include "core\Debug.h"

namespace lightEngine
{
	namespace graphics
	{
		Texture::~Texture()
		{
		}

		Texture* Texture::create(ImageData* imgData, TextureFilteringType textureFiltering, TextureImageMapping textureImageMapping, bool byteAlignmentRestriction)
		{
			switch (Graphics::get_graphics_API())
			{
			case GraphicsAPI::OpenGL: return new opengl::OpenglTexture(imgData, textureFiltering, textureImageMapping, byteAlignmentRestriction);

			default:
				break;
			}

			Debug::log("ERROR: Tried to create texture, but invalid graphics API was in use!");
			return nullptr;
		}
	}
}