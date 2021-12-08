#pragma once

#include "entities\Component.h"
#include "graphics\graphicsResources\Mesh.h"

namespace lightEngine
{
	namespace graphics
	{
		class Renderable3D : public Component
		{
		private:

			Mesh* _mesh;

		public:

			Renderable3D(Mesh* mesh);
			Renderable3D(Mesh* mesh, ComponentType extendedComponentType, size_t extendedComponentSize);
			virtual ~Renderable3D();

			inline const Mesh* getMesh() const { return _mesh; }
		};
	}
}