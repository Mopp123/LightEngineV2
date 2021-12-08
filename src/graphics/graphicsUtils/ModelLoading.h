#pragma once

#include "graphics\graphicsResources\Model.h"
#include <string>

namespace lightEngine
{
	namespace graphics
	{

		namespace modelLoading
		{
			enum ModelLoading_PostProcessFlags
			{
				None = 0x0,
				Triangulate = 0x1,
				FlipUVs = 0x2,
				CalcTangentSpace = 0x4,
				JoinIdenticalVertices = 0x8
			};


			/*
				*NOTE Sometimes you need to flip uvs for textures to be correctly..

				* If "loadMaterialData" == true
					- For each mesh this model has...
						- We load all textures this mesh has stored as its' diffuse, specular and normal maps.
						We load all material data of this mesh and create a new "Material resource" for it.
			*/
			Model* load_model(const std::string& path, unsigned int postProcessFlags, bool loadMaterialData);
		}
	}
}