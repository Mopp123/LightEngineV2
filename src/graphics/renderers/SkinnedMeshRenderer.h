#pragma once

#include "graphics\graphicsResources\Mesh.h"
#include "graphics\graphicsResources\Material.h"
#include "graphics\graphicsComponents\SkinnedRenderable3D.h"

/*
	*NOTE
		Currently we ONLY batch together geometry that uses the same mesh resource!
*/

#define MESH_RENDERER__INSTANCED_DATA_ENTRY_LENGTH 16

namespace lightEngine
{
	namespace graphics
	{

		class SkinnedMeshRenderer
		{
		protected:

			const unsigned int _maxBatchSize = 10000;

			std::vector<const SkinnedRenderable3D*> _renderQue;

		public:

			SkinnedMeshRenderer();
			virtual ~SkinnedMeshRenderer();

			virtual void submit(const SkinnedRenderable3D* renderable) = 0;
			virtual void flush() = 0;

			static SkinnedMeshRenderer* create();

		protected:

			void submitRenderableData(const SkinnedRenderable3D* renderable);
		};
	}
}