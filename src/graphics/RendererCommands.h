#pragma once

#include "utils\maths\Maths.h"
#include "graphicsResources\Mesh.h"

namespace lightEngine
{
	namespace graphics
	{

		class RendererCommands
		{
		protected:

			mml::Vec4 _clearColor;

		public:

			RendererCommands() {}

			virtual void clear() = 0;
			virtual void clearColor() = 0;

			virtual void bindMesh(const Mesh* mesh) const = 0;
			virtual void unbindMesh(const Mesh* mesh) const = 0;
			virtual void bindMaterial(const Material* material) const = 0;
			virtual void unbindMaterial() const = 0;

			inline void setClearColor(const mml::Vec4& color) { _clearColor = color; }

			static RendererCommands* create();
		};
	}
}