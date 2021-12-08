
#include "SkinnedMeshRenderer.h"
#include "entities\Entity.h"
#include "core\Debug.h"
#include "graphics\Graphics.h"
#include "graphics\opengl\openglRenderers\OpenglSkinnedMeshRenderer.h"

namespace lightEngine
{
	namespace graphics
	{

		SkinnedMeshRenderer::SkinnedMeshRenderer()
		{}

		SkinnedMeshRenderer::~SkinnedMeshRenderer()
		{}

		SkinnedMeshRenderer* SkinnedMeshRenderer::create()
		{
			switch (Graphics::get_graphics_API())
			{
			case GraphicsAPI::OpenGL: return new opengl::OpenglSkinnedMeshRenderer;

			default:
				break;
			}

			Debug::log("ERROR: Tried to create SkinnedMeshRenderer, but invalid graphics API was in use!");
			return nullptr;
		}


		void SkinnedMeshRenderer::submitRenderableData(const SkinnedRenderable3D* renderable)
		{
#ifdef LIGHT_ENGINE__DEBUG_MODE__FULL
			if (!renderable)
			{
				Debug::log(
					"ERROR: Location: SkinnedMeshRenderer::submitRenderableData(const SkinnedRenderable3D*)\n"
					"Tried to submit renderable but the renderable was nullptr!"
				);
				return;
			}
			if (!renderable->getMesh())
			{
				Debug::log(
					"ERROR: Location: SkinnedMeshRenderer::submitRenderableData(const SkinnedRenderable3D*)\n"
					"Tried to submit renderable but the renderable's Mesh was nullptr!"
				);
				return;
			}
#endif

			_renderQue.push_back(renderable);
		}

	}
}