
#include "FontRenderer.h"
#include "core\Application.h"
#include "core\Debug.h"
#include "entities\Entity.h"
#include "graphics\opengl\openglRenderers\OpenglFontRenderer.h"
#include <algorithm>

#include <ft2build.h>
#include FT_FREETYPE_H


namespace lightEngine
{
	namespace graphics
	{
		FontRenderer::FontRenderer()
		{
			// Create all glyphs we want
			std::vector<GlyphData> glyphs = createGlyphs("QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm_.:,!? 1234567890^'+-*/<>[]{}", "res/fonts/Cantarell-Regular.ttf");
			// Create "bitmapfont texture" from these glyphs
			createFontAtlas(glyphs);

			//_instancedBuffer_data.reserve(_maxCharacters * FONT_RENDERER__INSTANCED_DATA_ENTRY_LENGTH);
			_instancedBuffer_length = _maxCharacters * FONT_RENDERER__INSTANCED_DATA_ENTRY_LENGTH;
			_instancedBuffer_data = new float[_instancedBuffer_length];
			memset(_instancedBuffer_data, 0, sizeof(float) * _instancedBuffer_length);

			float vertices[] =
			{
				-1.0f, 1.0f,
				-1.0f, -1.0f,
				1.0f, 1.0f,
				1.0f, -1.0f
			};

			unsigned int indices[] =
			{
				0,1,2,3
			};

			IndexBuffer* ib = IndexBuffer::create(indices, 4, BufferUsage::StaticDraw);
			VertexBuffer* vb_vertexPositions = VertexBuffer::create(vertices, 8, BufferUsage::StaticDraw);
			VertexBuffer* vb_instancedData = VertexBuffer::create(
				sizeof(float) * FONT_RENDERER__INSTANCED_DATA_ENTRY_LENGTH * _maxCharacters, 
				BufferUsage::DynamicDraw
			);

			VertexBufferLayout layout_vertexPositions(
				{
					{ ShaderDataType::Float2, "position"}
				}
			);

			VertexBufferLayout layout_instancedData(
				{
					{ ShaderDataType::Float4, "transform", 1 },
					{ ShaderDataType::Float2, "texOffset", 1 },
					{ ShaderDataType::Float4, "color", 1 },
					{ ShaderDataType::Float, "layer", 1 }
				}
			);

			vb_vertexPositions->setLayout(layout_vertexPositions);
			vb_instancedData->setLayout(layout_instancedData);

			_mesh_font = Mesh::create(ib, { vb_vertexPositions, vb_instancedData }, PrimitiveDrawType::TriangleStrip);
		}
		
		FontRenderer::~FontRenderer()
		{
			delete _fontImageAtlas;
			delete _fontTexture;
			delete _mesh_font;
		}



		FontRenderer* FontRenderer::create()
		{
			switch (Graphics::get_graphics_API())
			{
			case GraphicsAPI::OpenGL: return new opengl::OpenglFontRenderer;

			default:
				break;
			}

			Debug::log("ERROR: Tried to create FontRenderer, but invalid graphics API was in use!");
			return nullptr;
		}

		// Updates the mesh's "instanced" vertex buffer that holds every gui image's transform data 
		// (Doesn't care about rendering api!)
		void FontRenderer::submitTextData(const gui::Text* text)
		{
#ifdef LIGHT_ENGINE__DEBUG_MODE__FULL
			if (!text)
			{
				Debug::log(
					"ERROR: Location: FontRenderer::submitTextData(const gui::Text*)\n"
					"Tried to submit Text but the Text was nullptr!"
				);
				return;
			}
			if (_count_characters + 1 > _maxCharacters)
			{
				Debug::log(
					"ERROR: Location: FontRenderer::submitTextData(const gui::Text*)\n"
					"Maximum character count reached!\n"
					"Currently maximum character count is " + std::to_string(_maxCharacters) + "\n" +
					"Current character count is " + std::to_string(_count_characters)
				);
				return;
			}
			// INSERT POINTER OUT OF BOUNDS ERROR CHECKING HERE!
#endif

			const std::string& str = text->getString();
			mml::Vec3 pos = text->getEntity()->getTransform().getGlobalPosition();
			makePixelAligned(pos.x);
			makePixelAligned(pos.y);

			const float originalX = pos.x;

			const float& fontWidth = text->getFontSize();
			const float& fontHeight = fontWidth;

			int currentLineLength = 0;
			bool enableDynamicLineSwitching = text->getMaxLineLength() > 0;
			bool blockLineSwitching = false;

			const float borderModifier = 1.3f; // ..?


			// Go through each character in the string
			for (const char& character : str) // <- if doesnt work try iterating with regularely (for(int i = 0 etc..)
			{
				const CharGraphicData& charData = _fontGraphicData[character];

				// If we exceed the text's max line length -> change line if we need to
				if (enableDynamicLineSwitching && !blockLineSwitching)
				{
					if (currentLineLength > text->getMaxLineLength())
						changeLine(currentLineLength, pos, originalX, fontHeight);
				}

				// Check, do we want to change line?
				if (character == '\n')
				{
					changeLine(currentLineLength, pos, originalX, fontHeight);
					continue;
				}

				// If this was just an empy space -> add to x and increase the line length
				if (character == ' ')
				{
					blockLineSwitching = false;
					advanceCursor(pos.x, charData.advance, fontWidth, borderModifier);
					currentLineLength++;
					continue;
				}
				blockLineSwitching = true;
				float x = pos.x + charData.bearingX * fontWidth;
				float y = pos.y - (charData.height - (charData.bearingY * 2.0f)) * fontHeight;
				
				float characterWidth = charData.width * fontWidth;
				float characterHeight = charData.height * fontHeight;

				makePixelAligned(x);
				makePixelAligned(y);

				makePixelAligned(characterWidth);
				makePixelAligned(characterHeight);


				_instancedBuffer_data[_instancedBuffer_pointer] = x;
				_instancedBuffer_data[_instancedBuffer_pointer + 1] = y;
				_instancedBuffer_data[_instancedBuffer_pointer + 2] = characterWidth;
				_instancedBuffer_data[_instancedBuffer_pointer + 3] = characterHeight;

				_instancedBuffer_data[_instancedBuffer_pointer + 4] = charData.texOffsetX;
				_instancedBuffer_data[_instancedBuffer_pointer + 5] = charData.texOffsetY;

				_instancedBuffer_data[_instancedBuffer_pointer + 6] = text->getColor().x;
				_instancedBuffer_data[_instancedBuffer_pointer + 7] = text->getColor().y;
				_instancedBuffer_data[_instancedBuffer_pointer + 8] = text->getColor().z;
				_instancedBuffer_data[_instancedBuffer_pointer + 9] = text->getColor().w;
				
				_instancedBuffer_data[_instancedBuffer_pointer + 10] = pos.z;

				// Move our "cursor" to the right
				advanceCursor(pos.x, charData.advance, fontWidth, borderModifier);
				
				currentLineLength++;

				_instancedBuffer_pointer += FONT_RENDERER__INSTANCED_DATA_ENTRY_LENGTH;
				_count_characters++;
			}
		}


		void FontRenderer::advanceCursor(float& cursorPos, const unsigned int& characterAdvance, const float& fontWidth, float borderModifier)
		{
			cursorPos += (float)(characterAdvance >> 6) * (fontWidth * 2 * borderModifier);
		}

		void FontRenderer::changeLine(int& currentLineLength, mml::Vec3& textPos, const float& textOriginX, const float& fontHeight) const
		{
			textPos.y -= (float)_maxCellHeight * (fontHeight * 2);
			textPos.x = textOriginX;
			makePixelAligned(textPos.x);
			makePixelAligned(textPos.y);

			currentLineLength = 0;
		}

		void FontRenderer::makePixelAligned(float& fv) const
		{
			fv = std::floor(fv) + 0.5f;
		}


	#define TEMP_FONT_LOADING__PIXEL_SIZES 32
		unsigned char* make_distance_map(unsigned char*, unsigned int, unsigned int);

		// Loads font and creates "glyph data" from inputted characters
		std::vector<GlyphData> FontRenderer::createGlyphs(std::string characters, std::string fontFilePath)
		{
			std::vector<GlyphData> glyphs;
			glyphs.reserve(characters.size());

			FT_Library ftLib;
			if (FT_Init_FreeType(&ftLib))
			{
				Debug::log("ERROR: Location: FontRenderer::createGlyphs(std::string, std::string)\nFailed to init FreeType library!");
				return glyphs;
			}

			// Attempt to load the font itself..
			FT_Face fontFace;
			if (FT_New_Face(ftLib, fontFilePath.c_str(), 0, &fontFace))
			{
				Debug::log("ERROR: Location: FontRenderer::createGlyphs(std::string, std::string)\nFailed to create FT_Face from :" + fontFilePath + "\nMake sure that you inputted the right file path!");
				FT_Done_FreeType(ftLib);
				return glyphs;
			}

			FT_Set_Pixel_Sizes(fontFace, 0, TEMP_FONT_LOADING__PIXEL_SIZES);

			// Iterate through each character in the inputted "characters" and store all required metrics for rendering this character
			std::string::const_iterator it;
			for (it = characters.begin(); it != characters.end(); it++)
			{
				char c = *it;
				if (FT_Load_Char(fontFace, c, FT_LOAD_RENDER))
				{
					Debug::log("ERROR: Location: FontRenderer::createGlyphs(std::string, std::string)\nFailed to load character!");
					continue;
				}

				unsigned int width = fontFace->glyph->bitmap.width;
				unsigned int height = fontFace->glyph->bitmap.rows;

				// We need to copy the bitmap for later use when we combine all glyphs into a single texture
				// * We need to add some empty space to the borders of the character, so we don't get weird artifacts
				// when rendering text with different size
				int padding = 16;
				unsigned int finalWidth = width + padding;
				unsigned int finalHeight = height + padding;

				unsigned char* bmp = new unsigned char[finalWidth * finalHeight];
				memset(bmp, 0, finalWidth * finalHeight);

				// Adding that empty space..
				for (int py = 0; py < height; py++)
					for (int px = 0; px < width; px++)
						bmp[(px + padding / 2) + (py + padding / 2) * finalWidth] = fontFace->glyph->bitmap.buffer[px + py * width];

				GlyphData gd = {
					c,
					bmp,
					finalWidth,
					finalHeight,
					fontFace->glyph->bitmap_left,
					fontFace->glyph->bitmap_top,
					fontFace->glyph->advance.x
				};

				glyphs.emplace_back(gd);
			}

			FT_Done_Face(fontFace);
			FT_Done_FreeType(ftLib);
			Debug::log("Font loaded successfully!");
			return glyphs;
		}
		
		// TESTING DISTANCE FIELD TEXT..
		// Copied from : https://github.com/raphm/makeglfont
		void computegradient(double *img, int w, int h, double *gx, double *gy)
		{
			int i, j, k; // ,p,q;
			double glength; //, phi, phiscaled, ascaled, errsign, pfrac, qfrac, err0, err1, err;
#define SQRT2 1.4142136
			for (i = 1; i < h - 1; i++) { // Avoid edges where the kernels would spill over
				for (j = 1; j < w - 1; j++) {
					k = i * w + j;
					if ((img[k] > 0.0) && (img[k] < 1.0)) { // Compute gradient for edge pixels only
						gx[k] = -img[k - w - 1] - SQRT2 * img[k - 1] - img[k + w - 1] + img[k - w + 1] + SQRT2 * img[k + 1] + img[k + w + 1];
						gy[k] = -img[k - w - 1] - SQRT2 * img[k - w] - img[k + w - 1] + img[k - w + 1] + SQRT2 * img[k + w] + img[k + w + 1];
						glength = gx[k] * gx[k] + gy[k] * gy[k];
						if (glength > 0.0) { // Avoid division by zero
							glength = std::sqrt(glength);
							gx[k] = gx[k] / glength;
							gy[k] = gy[k] / glength;
						}
					}
				}
			}
			// TODO: Compute reasonable values for gx, gy also around the image edges.
			// (These are zero now, which reduces the accuracy for a 1-pixel wide region
			// around the image edge.) 2x2 kernels would be suitable for this.
		}


		/*
 * A somewhat tricky function to approximate the distance to an edge in a
 * certain pixel, with consideration to either the local gradient (gx,gy)
 * or the direction to the pixel (dx,dy) and the pixel greyscale value a.
 * The latter alternative, using (dx,dy), is the metric used by edtaa2().
 * Using a local estimate of the edge gradient (gx,gy) yields much better
 * accuracy at and near edges, and reduces the error even at distant pixels
 * provided that the gradient direction is accurately estimated.
 */
		double edgedf(double gx, double gy, double a)
		{
			double df, glength, temp, a1;

			if ((gx == 0) || (gy == 0)) { // Either A) gu or gv are zero, or B) both
				df = 0.5 - a;  // Linear approximation is A) correct or B) a fair guess
			}
			else {
				glength = sqrt(gx*gx + gy * gy);
				if (glength > 0) {
					gx = gx / glength;
					gy = gy / glength;
				}
				/* Everything is symmetric wrt sign and transposition,
				 * so move to first octant (gx>=0, gy>=0, gx>=gy) to
				 * avoid handling all possible edge directions.
				 */
				gx = fabs(gx);
				gy = fabs(gy);
				if (gx < gy) {
					temp = gx;
					gx = gy;
					gy = temp;
				}
				a1 = 0.5*gy / gx;
				if (a < a1) { // 0 <= a < a1
					df = 0.5*(gx + gy) - sqrt(2.0*gx*gy*a);
				}
				else if (a < (1.0 - a1)) { // a1 <= a <= 1-a1
					df = (0.5 - a)*gx;
				}
				else { // 1-a1 < a <= 1
					df = -0.5*(gx + gy) + sqrt(2.0*gx*gy*(1.0 - a));
				}
			}
			return df;
		}

		double distaa3(double *img, double *gximg, double *gyimg, int w, int c, int xc, int yc, int xi, int yi)
		{
			double di, df, dx, dy, gx, gy, a;
			int closest;

			closest = c - xc - yc * w; // Index to the edge pixel pointed to from c
			a = img[closest];    // Grayscale value at the edge pixel
			gx = gximg[closest]; // X gradient component at the edge pixel
			gy = gyimg[closest]; // Y gradient component at the edge pixel

			if (a > 1.0) a = 1.0;
			if (a < 0.0) a = 0.0; // Clip grayscale values outside the range [0,1]
			if (a == 0.0) return 1000000.0; // Not an object pixel, return "very far" ("don't know yet")

			dx = (double)xi;
			dy = (double)yi;
			di = sqrt(dx*dx + dy * dy); // Length of integer vector, like a traditional EDT
			if (di == 0) { // Use local gradient only at edges
				// Estimate based on local gradient only
				df = edgedf(gx, gy, a);
			}
			else {
				// Estimate gradient based on direction to edge (accurate for large di)
				df = edgedf(dx, dy, a);
			}
			return di + df; // Same metric as edtaa2, except at edges (where di=0)
		}
		// Shorthand macro: add ubiquitous parameters dist, gx, gy, img and w and call distaa3()
#define DISTAA(c,xc,yc,xi,yi) (distaa3(img, gx, gy, w, c, xc, yc, xi, yi))

		void edtaa3(double *img, double *gx, double *gy, int w, int h, short *distx, short *disty, double *dist)
		{
			int x, y, i, c;
			int offset_u, offset_ur, offset_r, offset_rd,
				offset_d, offset_dl, offset_l, offset_lu;
			double olddist, newdist;
			int cdistx, cdisty, newdistx, newdisty;
			int changed;
			double epsilon = 1e-3;

			/* Initialize index offsets for the current image width */
			offset_u = -w;
			offset_ur = -w + 1;
			offset_r = 1;
			offset_rd = w + 1;
			offset_d = w;
			offset_dl = w - 1;
			offset_l = -1;
			offset_lu = -w - 1;

			/* Initialize the distance images */
			for (i = 0; i < w*h; i++) {
				distx[i] = 0; // At first, all pixels point to
				disty[i] = 0; // themselves as the closest known.
				if (img[i] <= 0.0)
				{
					dist[i] = 1000000.0; // Big value, means "not set yet"
				}
				else if (img[i] < 1.0) {
					dist[i] = edgedf(gx[i], gy[i], img[i]); // Gradient-assisted estimate
				}
				else {
					dist[i] = 0.0; // Inside the object
				}
			}

			/* Perform the transformation */
			do
			{
				changed = 0;

				/* Scan rows, except first row */
				for (y = 1; y < h; y++)
				{

					/* move index to leftmost pixel of current row */
					i = y * w;

					/* scan right, propagate distances from above & left */

					/* Leftmost pixel is special, has no left neighbors */
					olddist = dist[i];
					if (olddist > 0) // If non-zero distance or not set yet
					{
						c = i + offset_u; // Index of candidate for testing
						cdistx = distx[c];
						cdisty = disty[c];
						newdistx = cdistx;
						newdisty = cdisty + 1;
						newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
						if (newdist < olddist - epsilon)
						{
							distx[i] = newdistx;
							disty[i] = newdisty;
							dist[i] = newdist;
							olddist = newdist;
							changed = 1;
						}

						c = i + offset_ur;
						cdistx = distx[c];
						cdisty = disty[c];
						newdistx = cdistx - 1;
						newdisty = cdisty + 1;
						newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
						if (newdist < olddist - epsilon)
						{
							distx[i] = newdistx;
							disty[i] = newdisty;
							dist[i] = newdist;
							changed = 1;
						}
					}
					i++;

					/* Middle pixels have all neighbors */
					for (x = 1; x < w - 1; x++, i++)
					{
						olddist = dist[i];
						if (olddist <= 0) continue; // No need to update further

						c = i + offset_l;
						cdistx = distx[c];
						cdisty = disty[c];
						newdistx = cdistx + 1;
						newdisty = cdisty;
						newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
						if (newdist < olddist - epsilon)
						{
							distx[i] = newdistx;
							disty[i] = newdisty;
							dist[i] = newdist;
							olddist = newdist;
							changed = 1;
						}

						c = i + offset_lu;
						cdistx = distx[c];
						cdisty = disty[c];
						newdistx = cdistx + 1;
						newdisty = cdisty + 1;
						newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
						if (newdist < olddist - epsilon)
						{
							distx[i] = newdistx;
							disty[i] = newdisty;
							dist[i] = newdist;
							olddist = newdist;
							changed = 1;
						}

						c = i + offset_u;
						cdistx = distx[c];
						cdisty = disty[c];
						newdistx = cdistx;
						newdisty = cdisty + 1;
						newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
						if (newdist < olddist - epsilon)
						{
							distx[i] = newdistx;
							disty[i] = newdisty;
							dist[i] = newdist;
							olddist = newdist;
							changed = 1;
						}

						c = i + offset_ur;
						cdistx = distx[c];
						cdisty = disty[c];
						newdistx = cdistx - 1;
						newdisty = cdisty + 1;
						newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
						if (newdist < olddist - epsilon)
						{
							distx[i] = newdistx;
							disty[i] = newdisty;
							dist[i] = newdist;
							changed = 1;
						}
					}

					/* Rightmost pixel of row is special, has no right neighbors */
					olddist = dist[i];
					if (olddist > 0) // If not already zero distance
					{
						c = i + offset_l;
						cdistx = distx[c];
						cdisty = disty[c];
						newdistx = cdistx + 1;
						newdisty = cdisty;
						newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
						if (newdist < olddist - epsilon)
						{
							distx[i] = newdistx;
							disty[i] = newdisty;
							dist[i] = newdist;
							olddist = newdist;
							changed = 1;
						}

						c = i + offset_lu;
						cdistx = distx[c];
						cdisty = disty[c];
						newdistx = cdistx + 1;
						newdisty = cdisty + 1;
						newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
						if (newdist < olddist - epsilon)
						{
							distx[i] = newdistx;
							disty[i] = newdisty;
							dist[i] = newdist;
							olddist = newdist;
							changed = 1;
						}

						c = i + offset_u;
						cdistx = distx[c];
						cdisty = disty[c];
						newdistx = cdistx;
						newdisty = cdisty + 1;
						newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
						if (newdist < olddist - epsilon)
						{
							distx[i] = newdistx;
							disty[i] = newdisty;
							dist[i] = newdist;
							changed = 1;
						}
					}

					/* Move index to second rightmost pixel of current row. */
					/* Rightmost pixel is skipped, it has no right neighbor. */
					i = y * w + w - 2;

					/* scan left, propagate distance from right */
					for (x = w - 2; x >= 0; x--, i--)
					{
						olddist = dist[i];
						if (olddist <= 0) continue; // Already zero distance

						c = i + offset_r;
						cdistx = distx[c];
						cdisty = disty[c];
						newdistx = cdistx - 1;
						newdisty = cdisty;
						newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
						if (newdist < olddist - epsilon)
						{
							distx[i] = newdistx;
							disty[i] = newdisty;
							dist[i] = newdist;
							changed = 1;
						}
					}
				}

				/* Scan rows in reverse order, except last row */
				for (y = h - 2; y >= 0; y--)
				{
					/* move index to rightmost pixel of current row */
					i = y * w + w - 1;

					/* Scan left, propagate distances from below & right */

					/* Rightmost pixel is special, has no right neighbors */
					olddist = dist[i];
					if (olddist > 0) // If not already zero distance
					{
						c = i + offset_d;
						cdistx = distx[c];
						cdisty = disty[c];
						newdistx = cdistx;
						newdisty = cdisty - 1;
						newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
						if (newdist < olddist - epsilon)
						{
							distx[i] = newdistx;
							disty[i] = newdisty;
							dist[i] = newdist;
							olddist = newdist;
							changed = 1;
						}

						c = i + offset_dl;
						cdistx = distx[c];
						cdisty = disty[c];
						newdistx = cdistx + 1;
						newdisty = cdisty - 1;
						newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
						if (newdist < olddist - epsilon)
						{
							distx[i] = newdistx;
							disty[i] = newdisty;
							dist[i] = newdist;
							changed = 1;
						}
					}
					i--;

					/* Middle pixels have all neighbors */
					for (x = w - 2; x > 0; x--, i--)
					{
						olddist = dist[i];
						if (olddist <= 0) continue; // Already zero distance

						c = i + offset_r;
						cdistx = distx[c];
						cdisty = disty[c];
						newdistx = cdistx - 1;
						newdisty = cdisty;
						newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
						if (newdist < olddist - epsilon)
						{
							distx[i] = newdistx;
							disty[i] = newdisty;
							dist[i] = newdist;
							olddist = newdist;
							changed = 1;
						}

						c = i + offset_rd;
						cdistx = distx[c];
						cdisty = disty[c];
						newdistx = cdistx - 1;
						newdisty = cdisty - 1;
						newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
						if (newdist < olddist - epsilon)
						{
							distx[i] = newdistx;
							disty[i] = newdisty;
							dist[i] = newdist;
							olddist = newdist;
							changed = 1;
						}

						c = i + offset_d;
						cdistx = distx[c];
						cdisty = disty[c];
						newdistx = cdistx;
						newdisty = cdisty - 1;
						newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
						if (newdist < olddist - epsilon)
						{
							distx[i] = newdistx;
							disty[i] = newdisty;
							dist[i] = newdist;
							olddist = newdist;
							changed = 1;
						}

						c = i + offset_dl;
						cdistx = distx[c];
						cdisty = disty[c];
						newdistx = cdistx + 1;
						newdisty = cdisty - 1;
						newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
						if (newdist < olddist - epsilon)
						{
							distx[i] = newdistx;
							disty[i] = newdisty;
							dist[i] = newdist;
							changed = 1;
						}
					}
					/* Leftmost pixel is special, has no left neighbors */
					olddist = dist[i];
					if (olddist > 0) // If not already zero distance
					{
						c = i + offset_r;
						cdistx = distx[c];
						cdisty = disty[c];
						newdistx = cdistx - 1;
						newdisty = cdisty;
						newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
						if (newdist < olddist - epsilon)
						{
							distx[i] = newdistx;
							disty[i] = newdisty;
							dist[i] = newdist;
							olddist = newdist;
							changed = 1;
						}

						c = i + offset_rd;
						cdistx = distx[c];
						cdisty = disty[c];
						newdistx = cdistx - 1;
						newdisty = cdisty - 1;
						newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
						if (newdist < olddist - epsilon)
						{
							distx[i] = newdistx;
							disty[i] = newdisty;
							dist[i] = newdist;
							olddist = newdist;
							changed = 1;
						}

						c = i + offset_d;
						cdistx = distx[c];
						cdisty = disty[c];
						newdistx = cdistx;
						newdisty = cdisty - 1;
						newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
						if (newdist < olddist - epsilon)
						{
							distx[i] = newdistx;
							disty[i] = newdisty;
							dist[i] = newdist;
							changed = 1;
						}
					}

					/* Move index to second leftmost pixel of current row. */
					/* Leftmost pixel is skipped, it has no left neighbor. */
					i = y * w + 1;
					for (x = 1; x < w; x++, i++)
					{
						/* scan right, propagate distance from left */
						olddist = dist[i];
						if (olddist <= 0) continue; // Already zero distance

						c = i + offset_l;
						cdistx = distx[c];
						cdisty = disty[c];
						newdistx = cdistx + 1;
						newdisty = cdisty;
						newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
						if (newdist < olddist - epsilon)
						{
							distx[i] = newdistx;
							disty[i] = newdisty;
							dist[i] = newdist;
							changed = 1;
						}
					}
				}
			} while (changed); // Sweep until no more updates are made

			/* The transformation is completed. */

		}


		unsigned char * make_distance_map(unsigned char *img, unsigned int width, unsigned int height)
		{
			short * xdist = (short *)malloc(width * height * sizeof(short));
			short * ydist = (short *)malloc(width * height * sizeof(short));
			double * gx = (double *)calloc(width * height, sizeof(double));
			double * gy = (double *)calloc(width * height, sizeof(double));
			double * data = (double *)calloc(width * height, sizeof(double));
			double * outside = (double *)calloc(width * height, sizeof(double));
			double * inside = (double *)calloc(width * height, sizeof(double));
			int i;

			// Convert img into double (data)
			double img_min = 255, img_max = -255;
			for (i = 0; i < width*height; ++i)
			{
				double v = img[i];
				data[i] = v;
				if (v > img_max) img_max = v;
				if (v < img_min) img_min = v;
			}
			// Rescale image levels between 0 and 1
			for (i = 0; i < width*height; ++i)
			{
				data[i] = (img[i] - img_min) / img_max;
			}

			// Compute outside = edtaa3(bitmap); % Transform background (0's)
			computegradient(data, height, width, gx, gy);
			edtaa3(data, gx, gy, height, width, xdist, ydist, outside);
			for (i = 0; i < width*height; ++i)
				if (outside[i] < 0)
					outside[i] = 0.0;

			// Compute inside = edtaa3(1-bitmap); % Transform foreground (1's)
			memset(gx, 0, sizeof(double)*width*height);
			memset(gy, 0, sizeof(double)*width*height);
			for (i = 0; i < width*height; ++i)
				data[i] = 1 - data[i];
			computegradient(data, height, width, gx, gy);
			edtaa3(data, gx, gy, height, width, xdist, ydist, inside);
			for (i = 0; i < width*height; ++i)
				if (inside[i] < 0)
					inside[i] = 0.0;

			// distmap = outside - inside; % Bipolar distance field
			unsigned char *out = (unsigned char *)malloc(width * height * sizeof(unsigned char));
			for (i = 0; i < width*height; ++i)
			{
				outside[i] -= inside[i];
				outside[i] = 128 + outside[i] * 16;
				if (outside[i] < 0) outside[i] = 0;
				if (outside[i] > 255) outside[i] = 255;
				out[i] = 255 - (unsigned char)outside[i];
			}

			free(xdist);
			free(ydist);
			free(gx);
			free(gy);
			free(data);
			free(outside);
			free(inside);
			return out;
		}

		// Creates bitmap font texture atlas from created glyphs
		void FontRenderer::createFontAtlas(std::vector<GlyphData>& glyphs)
		{
			unsigned int cellWidth = 0;
			_maxCellHeight = 0;
			unsigned int bitmapWidth = 0;

			// First find the glyph that is the widest to get maxWidth and tallest to get maxHeight
			for (GlyphData& gd : glyphs)
			{
				cellWidth = std::max(cellWidth, gd.width);
				_maxCellHeight = std::max(_maxCellHeight, gd.height);
			}
			// We want each cell to be perfect square
			cellWidth = mml::get_next_pow2(std::max(cellWidth, _maxCellHeight));
			_maxCellHeight = mml::get_next_pow2(std::max(cellWidth, _maxCellHeight));

			// The amount of actual pixels formed from the glyphs
			unsigned int glyphsPixelCount = (cellWidth * _maxCellHeight) * glyphs.size();
			// But we require our textures to be always pow2s
			bitmapWidth = mml::get_next_pow2((unsigned int)std::sqrt((double)glyphsPixelCount));
			// And this will be the actual final size of the texture
			const unsigned int textureSize = bitmapWidth * bitmapWidth;
			unsigned char* bitmapFont = new unsigned char[textureSize];
			memset(bitmapFont, 0, textureSize);

			_fontAtlasRowCount = bitmapWidth / cellWidth;


			// Then combine all the glyphs into a single "image"
			// ..we go through each glyph .. "scanline style"
			for (int bpy = 0; bpy < bitmapWidth; bpy += cellWidth)
			{
				for (int bpx = 0; bpx < bitmapWidth; bpx += cellWidth)
				{
					int glyphIndexX = std::floor((double)bpx / (double)cellWidth);
					int glyphIndexY = std::floor((double)bpy / (double)cellWidth);
					int glyphsIndex = glyphIndexX + glyphIndexY * _fontAtlasRowCount;
					if (glyphsIndex < glyphs.size())
					{
						GlyphData& currentGlyph = glyphs[glyphsIndex];
						currentGlyph.texOffsetX = (float)glyphIndexX;
						currentGlyph.texOffsetY = (float)glyphIndexY;
						currentGlyph.visualWidth = currentGlyph.width + (cellWidth - currentGlyph.width);
						currentGlyph.visualHeight = currentGlyph.height + (_maxCellHeight - currentGlyph.height);


						// For each pixel in the glyph
						for (int gpy = 0; gpy < currentGlyph.height; gpy++)
						{
							for (int gpx = 0; gpx < currentGlyph.width; gpx++)
							{
								bitmapFont[(bpx + gpx) + (bpy + gpy) * bitmapWidth] = currentGlyph.bitmap[gpx + gpy * currentGlyph.width];
							}
						}


					}
				}
			}

			
			// Add distance field
			unsigned char * distanceFieldImg = make_distance_map(bitmapFont, bitmapWidth, bitmapWidth);

			_fontImageAtlas = new ImageData(distanceFieldImg, bitmapWidth, bitmapWidth, 1); // is flipped?

			// *NOTE it is up to the graphics api dependant implementation of this class to create the actual "graphics api texture"
			_fontTexture = Texture::create(_fontImageAtlas, TextureFilteringType::Linear, TextureImageMapping::ClampToEdge, false);


			// Now we can delete all those bitmaps
			for (GlyphData& gd : glyphs)
			{
				delete[] gd.bitmap;
				// Also, finally a quick hack to get this working with our current system..
				CharGraphicData c = { gd.visualWidth, gd.visualHeight, gd.bearingX, gd.bearingY, gd.advance, gd.texOffsetX, gd.texOffsetY };
				_fontGraphicData.insert(std::pair<char, CharGraphicData>(gd.character, c));
			}
		}

	}
}