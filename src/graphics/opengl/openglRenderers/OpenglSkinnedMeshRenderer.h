#pragma once

#include "graphics\renderers\SkinnedMeshRenderer.h"
#include "graphics\opengl\openglGraphicsResources\OpenglMesh.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenglSkinnedMeshRenderer : public SkinnedMeshRenderer
			{
			private:


			public:

				OpenglSkinnedMeshRenderer();
				virtual ~OpenglSkinnedMeshRenderer();

				virtual void submit(const SkinnedRenderable3D* renderable) override;
				virtual void flush() override;
			};
		}
	}
}