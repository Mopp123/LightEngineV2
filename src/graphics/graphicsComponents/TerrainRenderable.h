#pragma once

#include "Renderable3D.h"

namespace lightEngine
{
	namespace graphics
	{
		class TerrainRenderable : public Renderable3D
		{

		public:

			TerrainRenderable(Mesh* mesh);
			~TerrainRenderable();
		};
	}
}