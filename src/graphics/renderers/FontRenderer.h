#pragma once

#include "graphics\Renderer.h"
#include "graphics\graphicsResources\Mesh.h"
#include "GUI\Text.h"
#include "utils\image\ImageData.h"
#include "graphics\graphicsResources\Texture.h"
#include <map>

/*
	Instanced data goes following way here :

	x, y,
	width, height,
	texOffsetX, texOffsetY,
	color_r,
	color_g,
	color_b,
	color_a,
	pos.z(layer)

	= 11 floats
*/
#define FONT_RENDERER__INSTANCED_DATA_ENTRY_LENGTH 11

namespace lightEngine
{
	namespace graphics
	{
		struct GlyphData
		{
			unsigned char character;
			unsigned char* bitmap = nullptr;
			unsigned int width, height;
			int bearingX, bearingY;
			unsigned int advance;
			float texOffsetX, texOffsetY;
			unsigned int visualWidth, visualHeight;
		};

		// This is just a little smaller version of the glyph data..
		// "Graphical data" of a letter so we can draw it
		struct CharGraphicData
		{
			unsigned int width, height;
			int bearingX, bearingY;
			unsigned int advance;
			float texOffsetX, texOffsetY;
		};


		class FontRenderer
		{
		protected:

			// Every character uses the same mesh
			Mesh* _mesh_font = nullptr;
			ImageData* _fontImageAtlas = nullptr;
			Texture* _fontTexture = nullptr;

			// This contains "graphical data" of each character.. (The data we need to draw each possible character..)
			std::map<char, CharGraphicData> _fontGraphicData;

			/*
				data goes in following order..
				x, y, fontSize, 
			*/
			//std::vector<float> _instancedBuffer_data;
			float *_instancedBuffer_data = nullptr;

			unsigned int _fontAtlasRowCount;
			unsigned int _fontAtlasCellWidth;
			unsigned int _maxCellHeight;


			const unsigned int _maxCharacters = 10000;
			// Amount of submitted guiImages
			unsigned int _count_characters = 0;

			// points where we are currently going in the "instanced buffer". Needs to be reset in the end of flush()!
			unsigned int _instancedBuffer_length = 0;
			unsigned int _instancedBuffer_pointer = 0;

		public:

			FontRenderer();
			virtual ~FontRenderer();

			virtual void submit(const gui::Text* text) = 0;
			virtual void flush() = 0;

			static FontRenderer* create();

		protected:

			// Updates the mesh's "instanced" vertex buffer that holds every gui image's transform data 
			// (Doesn't care about rendering api!)
			void submitTextData(const gui::Text* text);

		private:

			// Loads font and creates "glyph data" from inputted characters
			std::vector<GlyphData> createGlyphs(std::string characters, std::string fontFilePath);

			// Creates bitmap font texture atlas from created glyphs
			void createFontAtlas(std::vector<GlyphData>& glyphs);

			void advanceCursor(float& cursorPos, const unsigned int& characterAdvance, const float& fontWidth, float borderModifier = 1.0f);
			void changeLine(int& currentLineLength, mml::Vec3& textPos, const float& textOriginX, const float& fontHeight) const;

			void makePixelAligned(float& fv) const;
		};
	}
}