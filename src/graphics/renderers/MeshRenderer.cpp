
#include "MeshRenderer.h"
#include "entities\Entity.h"
#include "core\Debug.h"
#include "graphics\Graphics.h"
#include "graphics\opengl\openglRenderers\OpenglMeshRenderer.h"

namespace lightEngine
{
	namespace graphics
	{


		/*
			Current instanced data = 

				transformation matrix(float[16])
		*/
		MeshRenderer::MeshRenderer() 
		{}

		MeshRenderer::~MeshRenderer()
		{}

		MeshRenderer* MeshRenderer::create()
		{
			switch (Graphics::get_graphics_API())
			{
			case GraphicsAPI::OpenGL: return new opengl::OpenglMeshRenderer;

			default:
				break;
			}

			Debug::log("ERROR: Tried to create MeshRenderer, but invalid graphics API was in use!");
			return nullptr;
		}


		void MeshRenderer::submitRenderableData(const Renderable3D* renderable)
		{
#ifdef LIGHT_ENGINE__DEBUG_MODE__FULL
			if (!renderable)
			{
				Debug::log(
					"ERROR: Location: MeshRenderer::submitRenderableData(const Renderable3D*)\n"
					"Tried to submit renderable but the renderable was nullptr!"
				);
				return;
			}
			if (!renderable->getMesh())
			{
				Debug::log(
					"ERROR: Location: MeshRenderer::submitRenderableData(const Renderable3D*)\n"
					"Tried to submit renderable but the renderable's Mesh was nullptr!"
				);
				return;
			}
#endif

			insertToBatch(renderable);
		}


		// Tries to find already existing batch for inputted renderable depending on its' mesh.
		// If batch was found -> adds this renderable to that batch.
		// If batch wasn't found -> creates new batch for renderables with the same mesh.
		void MeshRenderer::insertToBatch(const Renderable3D* renderable)
		{
			for (std::pair<const Mesh*, std::vector<float>>& batch : _batches)
			{
				if (batch.first == renderable->getMesh())
				{
					std::vector<float>& batchData = batch.second;
					if (batchData.size() < _maxBatchSize)
					{
						storeInstancedData(renderable, batchData);
						return;
					}
				}
			}

			createNewBatch(renderable);
		}

		// Creates new batch for renderable
		void MeshRenderer::createNewBatch(const Renderable3D* renderable)
		{
			std::vector<float> data;
			data.reserve(_maxBatchSize * MESH_RENDERER__INSTANCED_DATA_ENTRY_LENGTH);

			storeInstancedData(renderable, data);

			_batches.emplace_back(std::make_pair(renderable->getMesh(), data));
		}

		// Stores data from the renderable we want to put into our instanced buffer..
		void MeshRenderer::storeInstancedData(const Renderable3D* renderable, std::vector<float>& storage)
		{
			const float* m = renderable->getEntity()->getTransform().getTransformationMatrix().getRawArray();

			for (int i = 0; i < 16; i++)
				storage.emplace_back(m[i]);
		}

	}
}