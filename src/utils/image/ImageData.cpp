
#include "ImageData.h"

namespace lightEngine
{

	ImageData::ImageData(unsigned char* pixels, int width, int height, int channels, bool isFlipped) : 
		_imgData(pixels), 
		_width(width), _height(height), _channels(channels),
		_isFlipped(isFlipped)
	{
		_hasAlpha = _channels >= 4;
	}

	/*
	ImageData::ImageData(const ImageData& imgData) :
		_imgData(imgData._imgData),
		_width(imgData._width), _height(imgData._height), _channels(imgData._channels),
		_isFlipped(imgData._isFlipped)
	{
		_hasAlpha = _channels >= 4;
	}*/

	

	ImageData::~ImageData()
	{
		clearData();
	}

	void ImageData::clearData()
	{
		delete[] _imgData;
		_imgData = nullptr;
	}


	mml::Vec4 ImageData::getPixelColor(unsigned int x, unsigned int y) const
	{
		return mml::Vec4(0, 0, 0, 1);
	}
	void ImageData::setPixelColor(const mml::Vec4& color, unsigned int x, unsigned int y)
	{

	}
}