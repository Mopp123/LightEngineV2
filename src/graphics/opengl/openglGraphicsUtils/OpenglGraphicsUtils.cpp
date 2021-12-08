
#include "OpenglGraphicsUtils.h"
#include "core\Debug.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{
			GLenum convert_prim_draw_type_to_gl(PrimitiveDrawType type)
			{
				switch (type)
				{
				case PrimitiveDrawType::Triangles: return GL_TRIANGLES;
				case PrimitiveDrawType::TriangleStrip: return GL_TRIANGLE_STRIP;
				case PrimitiveDrawType::Lines: return GL_LINES;
				case PrimitiveDrawType::LineLoop: return GL_LINE_LOOP;
				case PrimitiveDrawType::Points: return GL_POINTS;
				default:
					break;
				}

				#ifdef LIGHT_ENGINE__DEBUG_MODE__FULL
					Debug::log("ERROR: Location: OpenglSkinnedMeshRenderer: GLenum convert_prim_draw_type_to_gl(PrimitiveDrawType)\nInvalid PrimitiveDrawType!");
				#endif

				return GL_NONE;
			}
		}
	}
}