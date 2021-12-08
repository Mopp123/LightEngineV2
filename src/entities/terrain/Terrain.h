#pragma once

#include "entities\Entity.h"
#include "graphics\graphicsResources\Mesh.h"
#include "graphics\graphicsResources\Material.h"
#include <vector>

/*
	*NOTE Terrains owns their mesh. ResourceManager has nothing to do with that mesh
*/
namespace lightEngine
{
	class Terrain : public Entity
	{
	private:

		std::vector<float> _heightmap;

		uint32_t _tilesPerRow;
		float _tileSize;

		graphics::Mesh* _mesh;
		graphics::TerrainRenderable* _renderable;

	public:

		Terrain(const std::vector<float>& heightmap, float tileSize, graphics::Material* material);
		~Terrain();

		inline void setMaterial(graphics::Material* material) { _mesh->setMaterial(material); }

		inline const graphics::Mesh* getMesh() const { return _mesh; }
		inline const graphics::TerrainRenderable* getRenderable() const { return _renderable; }

	private:
		void generate(const std::vector<float>& heightmap);
	};
}