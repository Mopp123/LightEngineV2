
#include <glew.h>
#include "OpenglTexture.h"
#include "core\Debug.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{

			static GLint get_gl_texture_min_filter(TextureFilteringType textureFiltering)
			{
				switch (textureFiltering)
				{
				case TextureFilteringType::Linear :			return GL_LINEAR;
				case TextureFilteringType::Nearest :		return GL_NEAREST;
				case TextureFilteringType::LinearMipmap :	return GL_LINEAR_MIPMAP_LINEAR;
				case TextureFilteringType::NearestMipmap :	return GL_NEAREST_MIPMAP_NEAREST;
				default:
					break;
				}

				Debug::log("ERROR: Location: get_gl_texture_min_filter(TextureFilteringType)! Invalid texture filtering type!");
				return GL_NONE;
			}

			static GLint get_gl_texture_mag_filter(TextureFilteringType textureFiltering)
			{
				switch (textureFiltering)
				{
				case TextureFilteringType::Linear :			return GL_LINEAR;
				case TextureFilteringType::Nearest :		return GL_NEAREST;
				case TextureFilteringType::LinearMipmap :	return GL_LINEAR;
				case TextureFilteringType::NearestMipmap :	return GL_NEAREST;
				default:
					break;
				}

				Debug::log("ERROR: Location: get_gl_texture_mag_filter(TextureFilteringType)! Invalid texture filtering type!");
				return GL_NONE;
			}

			static GLint get_gl_texture_wrap(TextureImageMapping textureImageMapping)
			{
				switch (textureImageMapping)
				{
				case TextureImageMapping::Repeat:			return GL_REPEAT;
				case TextureImageMapping::ClampToBorder:	return GL_CLAMP_TO_BORDER;
				case TextureImageMapping::ClampToEdge:		return GL_CLAMP_TO_EDGE;
				default:
					break;
				}

				Debug::log("ERROR: Location: get_gl_texture_wrap(TextureImageMapping)! Invalid texture image mapping type!");
				return GL_NONE;
			}


			static bool filtering_is_mipmapped(TextureFilteringType textureFiltering)
			{
				return	(textureFiltering == TextureFilteringType::LinearMipmap) || 
						(textureFiltering == TextureFilteringType::NearestMipmap);
			}



			// *NOTE ownership of the image data DOESN'T CHANGE HERE! (ImageData should be owned by the "ResourceManager")
			OpenglTexture::OpenglTexture(ImageData* imgData, TextureFilteringType textureFiltering, TextureImageMapping textureImageMapping, bool byteAlignmentRestriction) :
				Texture(imgData, textureFiltering, textureImageMapping)
			{
				// This is quite shit -> figure out better way later..
				/* Small explanation from learnopengl.com text rendering tutorial : 
					OpenGL requires that textures all have a 4-byte alignment e.g. their size is always a multiple of 4 bytes. 
					Normally this won't be a problem since most textures have a width that is a multiple of 4 and/or use 4 bytes 
					per pixel, but if we only use a single byte per pixel, the texture can have any possible width. By setting 
					its unpack alignment to 1 we ensure there are no alignment issues (which could cause segmentation faults).
				*/
				if (!byteAlignmentRestriction)
					glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte alignment restriction
				else
					glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

				GLint textureMinFilter = get_gl_texture_min_filter(textureFiltering);
				GLint textureMagFilter = get_gl_texture_mag_filter(textureFiltering);
				GLint textureWrapping = get_gl_texture_wrap(textureImageMapping);

				// Figure out our format..
				GLint internalFormat = GL_RGB;
				GLint format = GL_RGB;
				if (imgData->getChannels() == 1)
				{
					internalFormat = GL_RED;
					format = GL_RED;
				}
				else if (imgData->getChannels() == 4)
				{
					internalFormat = GL_RGBA;
					format = GL_RGBA;
				}

				glCreateTextures(GL_TEXTURE_2D, 1, &_id);
				glBindTexture(GL_TEXTURE_2D, _id);

				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, imgData->getWidth(), imgData->getHeight(), 0, format, GL_UNSIGNED_BYTE, imgData->getImgData());

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapping);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapping);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureMinFilter);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureMagFilter);

				if (filtering_is_mipmapped(textureFiltering))
				{
					glGenerateMipmap(GL_TEXTURE_2D);
				}

				glBindTexture(GL_TEXTURE_2D, 0);
			}

			OpenglTexture::~OpenglTexture()
			{
				glDeleteTextures(1, &_id);
			}

			// Updates the "graphics api" texture according to its' image.
			// For example this can be used to make texture change when its' underlying image gets changed. 
			// (Some dynamic terrain blendmap painting for example..)
			void OpenglTexture::updateFromImage()
			{
				glBindTexture(GL_TEXTURE_2D, _id);
				glTexImage2D(
					GL_TEXTURE_2D, 
					0, 
					GL_BGR, 
					_imageData->getWidth(), 
					_imageData->getHeight(), 
					0, 
					GL_RGB, 
					GL_UNSIGNED_BYTE, 
					_imageData->getImgData()
				);
				glBindTexture(GL_TEXTURE_2D, 0);

			}
		}
	}
}