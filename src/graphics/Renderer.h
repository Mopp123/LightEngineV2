#pragma once

#include "graphics\graphicsResources\Mesh.h"
#include <vector>

namespace lightEngine
{
	namespace graphics
	{
		
		class Renderer
		{
		protected:

			std::vector<const Mesh*> _renderQue;

		public:

			Renderer() {}
			virtual ~Renderer() {}

			virtual void submit(const Mesh* mesh) = 0;
			virtual void flush() = 0;

		};
	}
}