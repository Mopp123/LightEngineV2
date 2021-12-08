#pragma once

#include "ImageData.h"
#include <string>

namespace lightEngine
{
	namespace imageLoading
	{
		ImageData* load_image(const std::string& path, bool flipY = false);
	}
}