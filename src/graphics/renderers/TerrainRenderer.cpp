
#include "TerrainRenderer.h"
#include "graphics\Graphics.h"
#include "graphics\opengl\openglRenderers\OpenglTerrainRenderer.h"
#include "core\Debug.h"

namespace lightEngine
{
	namespace graphics
	{


		TerrainRenderer::TerrainRenderer() {}
		TerrainRenderer::~TerrainRenderer() {}

		TerrainRenderer* TerrainRenderer::create()
		{
			switch (Graphics::get_graphics_API())
			{
			case GraphicsAPI::OpenGL: return new opengl::OpenglTerrainRenderer;

			default:
				break;
			}

			Debug::log("ERROR: Tried to create TerrainRenderer, but invalid graphics API was in use!");
			return nullptr;
		}
	}
}