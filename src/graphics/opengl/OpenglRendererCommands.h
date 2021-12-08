#pragma once


#include "graphics\RendererCommands.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenglRendererCommands : public RendererCommands
			{
			public:

				virtual void clear() override;
				virtual void clearColor() override;
				virtual void bindMesh(const Mesh* mesh) const override;
				virtual void unbindMesh(const Mesh* mesh) const override;
				virtual void bindMaterial(const Material* material) const override;
				virtual void unbindMaterial() const override;
			};
		}
	}
}