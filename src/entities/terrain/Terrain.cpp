
#include "Terrain.h"
#include <algorithm>

namespace lightEngine
{
	Terrain::Terrain(const std::vector<float>& heightmap, float tileSize, graphics::Material* material) :
		Entity("Terrain"),
		_heightmap(heightmap),
		_tileSize(tileSize)
	{
		_tilesPerRow = std::sqrt(heightmap.size()) - 1;
		generate(_heightmap);

		_mesh->setMaterial(material);
	}

	Terrain::~Terrain()
	{
		delete _mesh;
		_mesh = nullptr;
	}


	void Terrain::generate(const std::vector<float>& heightmap)
	{
		uint32_t verticesPerRow = _tilesPerRow + 1;

		std::vector<mml::Vec3> vertexPositions;
		std::vector<mml::Vec2> vertexTexCoords;
		std::vector<mml::Vec3> vertexNormals;

		// Generate vertex positions
		uint32_t vertexCount = 0;

		float totalWidth = _tileSize * verticesPerRow;

		for (int x = 0; x < verticesPerRow; x++)
		{
			for (int z = 0; z < verticesPerRow; z++)
			{
				mml::Vec3 vertexPos(x * _tileSize, heightmap[x + z * verticesPerRow], -z * _tileSize);
				vertexPositions.push_back(vertexPos);
				
				mml::Vec2 texCoord(vertexPos.x / totalWidth, vertexPos.z / totalWidth);
				vertexTexCoords.push_back(texCoord);

				// Figure out normal (quite shit and unpercise way, but it looks fine for now..)
				float up = 0.0f;
				float down = 0.0f;
				float left = 0.0f;
				float right = 0.0f;
				
				if (z + 1 < verticesPerRow) up =	heightmap[x + (z + 1) * verticesPerRow];
				if (z - 1 >= 0)				down =	heightmap[x + (z - 1) * verticesPerRow];
				if (x - 1 >= 0)				left =	heightmap[(x - 1) + z * verticesPerRow];
				if (x + 1 < verticesPerRow) right = heightmap[(x + 1) + z * verticesPerRow];

				float nx = left - right;
				float nz = up - down;
				
				mml::Vec3 normal(nx, 0, nz);
				normal.normalize();
				normal.x *= 0.5f;
				normal.z *= 0.5f;
				normal.y = 1.0f;
				
				vertexNormals.push_back(normal);

				vertexCount++;
			}
		}

		// Combine all into 1 buffer
		std::vector<float> vertexData;
		vertexData.reserve(3 * vertexCount * 3);
		for (int i = 0; i < vertexCount; i++)
		{
			vertexData.emplace_back(vertexPositions[i].x);
			vertexData.emplace_back(vertexPositions[i].y);
			vertexData.emplace_back(vertexPositions[i].z);

			vertexData.emplace_back(vertexTexCoords[i].x);
			vertexData.emplace_back(vertexTexCoords[i].y);

			vertexData.emplace_back(vertexNormals[i].x);
			vertexData.emplace_back(vertexNormals[i].y);
			vertexData.emplace_back(vertexNormals[i].z);
		}


		// Generate indices (Couldn't figure out how to do this with triangle strips, without getting artifacts on "back side" of the mesh)
		std::vector<uint32_t> indices;
		for (uint32_t x = 0; x < verticesPerRow; x++)
		{
			for (uint32_t z = 0; z < verticesPerRow; z++)
			{
				if (x >= verticesPerRow - 1 || z >= verticesPerRow - 1)
					continue;

				uint32_t topLeft = x + (z + 1) * verticesPerRow;
				uint32_t bottomLeft = x + z * verticesPerRow;

				uint32_t topRight = (x + 1) + (z + 1) * verticesPerRow;
				uint32_t bottomRight = (x + 1) + z * verticesPerRow;


				indices.push_back(bottomLeft);
				indices.push_back(topLeft);
				indices.push_back(topRight);

				indices.push_back(topRight);
				indices.push_back(bottomRight);
				indices.push_back(bottomLeft);
			}
		}


		graphics::VertexBuffer* vb = graphics::VertexBuffer::create(&vertexData[0], vertexData.size(), graphics::BufferUsage::StaticDraw);
		graphics::IndexBuffer* ib = graphics::IndexBuffer::create(&indices[0], indices.size(), graphics::BufferUsage::StaticDraw);

		graphics::VertexBufferLayout vb_layout({
			{ graphics::ShaderDataType::Float3, "position" },
			{ graphics::ShaderDataType::Float2, "texCoord" },
			{ graphics::ShaderDataType::Float3, "normal" }
		});

		vb->setLayout(vb_layout);

		_mesh = graphics::Mesh::create(ib, { vb }, graphics::PrimitiveDrawType::LineLoop);
		_renderable = new graphics::TerrainRenderable(_mesh);

		addComponent(_renderable);
	}
}