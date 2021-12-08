#pragma once

#include "graphics\Buffers.h"
#include <vector>

#include "graphics\graphicsResources\Material.h"

namespace lightEngine
{
	namespace animation
	{
		class Skeleton;
	}

	namespace graphics
	{
		enum class PrimitiveDrawType
		{
			None = 0, Triangles, TriangleStrip, Lines, LineLoop, Points
		};

		class Mesh
		{
		protected:

			IndexBuffer* _indexBuffer;
			std::vector<VertexBuffer*> _vertexBuffers;
			PrimitiveDrawType _drawType = PrimitiveDrawType::Triangles;

			Material* _material = nullptr;

			animation::Skeleton* _skeleton = nullptr;

		public:

			Mesh() {}
			Mesh(PrimitiveDrawType drawType);
			virtual ~Mesh();

			static Mesh* create(IndexBuffer* indexBuffer, std::vector<VertexBuffer*> vertexBuffers, PrimitiveDrawType drawType);

			// Creates instance of a skeleton (if this mesh was a skinned mesh)
			animation::Skeleton* createSkeletonInstance();


			inline void setMaterial(Material* material) { _material = material; }

			inline const IndexBuffer* const getIndexBuffer() const { return _indexBuffer; }
			inline const std::vector<VertexBuffer*>& getVertexBuffers() const { return _vertexBuffers; }
			inline const PrimitiveDrawType& getDrawType() const { return _drawType; }

			inline Material* getMaterial() const { return _material; }
			
			// Do all graphics api dependent memory freeing etc in here...
			virtual void destroy() {}

			inline void setSkeletonPrototype(animation::Skeleton* prototype) { _skeleton = prototype; }
			inline animation::Skeleton* getSkeletonPrototype() { return _skeleton; }
		};
	}
}