#pragma once

#include "Mesh.h"

namespace lightEngine
{
	namespace graphics
	{
		class Model
		{
		private:

			std::vector<Mesh*> _meshes;

		public:

			Model() {}

			~Model()
			{
				for (Mesh* m : _meshes)
					delete m;
			}

			void addMesh(Mesh* m)
			{
				_meshes.push_back(m);
			}

			inline const std::vector<Mesh*>& getMeshes() const { return _meshes; }

		};
	}
}