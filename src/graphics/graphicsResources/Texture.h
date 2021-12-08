#pragma once

#include "utils\image\ImageData.h"

namespace lightEngine
{
	namespace graphics
	{

		// Linear : The kind of "smooth filtering" without mipmapping
		// Nearest : "Pixelated filtering" without mipmapping
		// LinearMipmap : same as "Linear" but with mipmapping
		// NearestMipmap : same as "Nearest" but with mipmapping
		enum class TextureFilteringType
		{
			None = 0, Linear, Nearest, LinearMipmap, NearestMipmap
		};

		enum class TextureImageMapping
		{
			Repeat = 0, ClampToBorder, ClampToEdge
		};


		class Texture
		{
		protected:
			ImageData* _imageData = nullptr;
			unsigned int _id;

		public:

			// *NOTE ownership of the image data DOESN'T CHANGE HERE! (ImageData should be owned by the "ResourceManager")
			Texture(ImageData* imgData, TextureFilteringType textureFiltering, TextureImageMapping textureImageMapping) : _imageData(imgData) {}
			virtual ~Texture();

			static Texture* create(ImageData* imgData, TextureFilteringType textureFiltering, TextureImageMapping textureImageMapping, bool byteAlignmentRestriction = true);

			// Updates the "graphics api" texture according to its' image.
			// For example this can be used to make texture change when its' underlying image gets changed. 
			// (Some dynamic terrain blendmap painting for example..)
			// *NOTE Texture filtering cannot currently change after the creation of the texture instance!
			virtual void updateFromImage() {}

			// Changes the image data this texture uses
			inline void setImageData(ImageData* imgData) { _imageData = imgData; }
			inline ImageData* getImageData() { return _imageData; }

			inline const unsigned int& getID() const { return _id; }
		};
	}
}