
#include "TerrainRenderable.h"

namespace lightEngine
{
	namespace graphics
	{

		TerrainRenderable::TerrainRenderable(Mesh* mesh) : 
			Renderable3D(mesh, ComponentType::RenderableTerrain, sizeof(*this))
		{}

		TerrainRenderable::~TerrainRenderable()
		{}
	}
}