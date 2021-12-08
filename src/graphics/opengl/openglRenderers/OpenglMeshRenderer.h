#pragma once

#include "graphics\renderers\MeshRenderer.h"
#include "graphics\opengl\openglGraphicsResources\OpenglMesh.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenglMeshRenderer : public MeshRenderer
			{
			private:


			public:

				OpenglMeshRenderer();
				virtual ~OpenglMeshRenderer();

				virtual void submit(const Renderable3D* renderable) override;
				virtual void flush() override;
			};
		}
	}
}