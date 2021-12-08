#pragma once

#include "graphics\graphicsComponents\TerrainRenderable.h"

namespace lightEngine
{
	namespace graphics
	{
		class TerrainRenderer
		{
		protected:

			std::vector<const TerrainRenderable*> _renderQue;

		public:

			TerrainRenderer();
			virtual ~TerrainRenderer();

			virtual void submit(const TerrainRenderable* renderable) = 0;
			virtual void flush() = 0;

			static TerrainRenderer* create();
		};
	}
}