
#include "ImageLoading.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "core\Debug.h"

namespace lightEngine
{
	namespace imageLoading
	{
		ImageData* load_image(const std::string& path, bool flipY)
		{
			int width = 0;
			int height = 0;
			int channels = 0;
			
			stbi_set_flip_vertically_on_load(flipY);

			unsigned char* imgData = stbi_load(path.c_str(), &width, &height, &channels, 0);

			if (!imgData)
				Debug::log(
					"ERROR: Location : imageLoading::load_image(cosnt std::string&, bool)\n"
					"Failed to load image from location : " + path
				);
			//This propably should be called, but it apparently just calls "free(imgData)", but we don't want to do that yet, since the ownership of the imgData gets transferred to the created ImageData instance!?
			//stbi_image_free(imgData);
			return new ImageData(imgData, width, height, channels, flipY);
		}
	}
}