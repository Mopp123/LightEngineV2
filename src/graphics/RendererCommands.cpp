
#include "RendererCommands.h"
#include "opengl\OpenglRendererCommands.h"
#include "Graphics.h"
#include "core\Debug.h"

namespace lightEngine
{
	namespace graphics
	{
		RendererCommands* RendererCommands::create()
		{
			switch (Graphics::get_graphics_API())
			{
			case GraphicsAPI::OpenGL: return new opengl::OpenglRendererCommands;

			default:
				break;
			}

			Debug::log("ERROR: Tried to create RendererCommands, but invalid graphics API was in use!");
			return nullptr;
		}
	}
}