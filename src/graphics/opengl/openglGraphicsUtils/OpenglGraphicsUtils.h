#pragma once

#include <glfw3.h> // included glfw here, because we cannot include glew.. since we need GLenums here..
#include "graphics\graphicsResources\Mesh.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{
			GLenum convert_prim_draw_type_to_gl(PrimitiveDrawType type);
		}
	}
}