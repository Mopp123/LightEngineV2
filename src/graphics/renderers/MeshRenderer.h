#pragma once

#include "graphics\graphicsResources\Mesh.h"
#include "graphics\graphicsResources\Material.h"
#include "graphics\graphicsComponents\Renderable3D.h"


/*
	*NOTE 
		Currently we ONLY batch together geometry that uses the same mesh resource!
*/

#define MESH_RENDERER__INSTANCED_DATA_ENTRY_LENGTH 16

namespace lightEngine
{
	namespace graphics
	{

		struct InstancedMeshData
		{
			float transformationMatrix[16];
		};

		class MeshRenderer
		{
		protected:

			const unsigned int _maxBatchCount = 100;
			const unsigned int _maxBatchSize = 10000;

			std::vector<std::pair<const Mesh*, std::vector<float>>> _batches;

		public:

			MeshRenderer();
			virtual ~MeshRenderer();

			virtual void submit(const Renderable3D* renderable) = 0;
			virtual void flush() = 0;

			static MeshRenderer* create();

		protected:

			void submitRenderableData(const Renderable3D* renderable);

			// Tries to find already existing batch for inputted renderable depending on its' mesh.
			// If batch was found -> adds this renderable to that batch.
			// If batch wasn't found -> creates new batch for renderables with the same mesh.
			void insertToBatch(const Renderable3D* renderable);

			// Creates new batch for renderable
			void createNewBatch(const Renderable3D* renderable);

			// Stores data from the renderable we want to put into our instanced buffer..
			void storeInstancedData(const Renderable3D* renderable, std::vector<float>& storage);


		};
	}
}