#pragma once

#include "graphics\graphicsResources\Texture.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenglTexture : public Texture
			{
			public:

				// *NOTE ownership of the image data DOESN'T CHANGE HERE! (ImageData should be owned by the "ResourceManager")
				OpenglTexture(ImageData* imgData, TextureFilteringType textureFiltering, TextureImageMapping textureImageMapping, bool byteAlignmentRestriction = true);
				~OpenglTexture();

				// Updates the "graphics api" texture according to its' image.
				// For example this can be used to make texture change when its' underlying image gets changed. 
				// (Some dynamic terrain blendmap painting for example..)
				// *NOTE Texture filtering cannot currently change after the creation of the texture instance!
				virtual void updateFromImage() override;
			};
		}
	}
}