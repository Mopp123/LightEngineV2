#pragma once

#include "utils\maths\Maths.h"

namespace lightEngine
{
	class ImageData
	{
	private:

		// img data means the image's pixels..
		unsigned char* _imgData = nullptr;
		int _width = -1;
		int _height = -1;
		int _channels = -1;

		bool _hasAlpha = false;
		bool _isFlipped = false;

	public:

		ImageData() {}
		// *NOTE the ownership of pixels gets transferred to this image instance!
		//	When instance of ImageData gets destroyed 
		//		-> it also destroys the "pixels" / imgData it owns!
		//		(if you delete the array of pixels this ImageData owns before 
		//		this ImageData's destructor is called we may get in deep shit!)
		ImageData(unsigned char* pixels, int width, int height, int channels, bool isFlipped = false);
		//ImageData(const ImageData& imgData);

		~ImageData();
		void clearData();

		mml::Vec4 getPixelColor(unsigned int x, unsigned int y) const;
		void setPixelColor(const mml::Vec4& color, unsigned int x, unsigned int y);

		inline unsigned char* getImgData() { return _imgData; }
		inline int getWidth() const { return _width; }
		inline int getHeight() const { return _height; }
		inline int getChannels() const { return _channels; }

		inline bool hasAlpha() const { return _hasAlpha; }
		inline bool isFlipped() const { return _isFlipped; }
	};
}