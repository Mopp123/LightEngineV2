#pragma once

#include "graphics\opengl\openglGraphicsResources\OpenglMesh.h"
#include "graphics\renderers\TerrainRenderer.h"
#include "graphics\opengl\openglShaders\OpenglShader.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenglTerrainRenderer : public TerrainRenderer
			{
			private:

				OpenglShader* _shader = nullptr;

			public:

				OpenglTerrainRenderer();
				virtual ~OpenglTerrainRenderer();

				virtual void submit(const TerrainRenderable* renderable) override;
				virtual void flush() override;

			private:

				void bindTextures(const Material* material) const;
			};
		}
	}
}