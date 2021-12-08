#pragma once

#include <vector>
#include <string>
#include <map>

#include "utils\image\ImageData.h"
#include "graphics\graphicsResources\Texture.h"
#include "graphics\graphicsResources\Material.h"
#include "graphics\graphicsResources\Model.h"
#include "animations\skeletalAnimation\Skeleton.h"

namespace lightEngine
{
	class ResourceManager
	{
	private:

		static std::vector<std::pair<std::string, ImageData*>>				s_resources_dynamic_images;
		static std::vector<std::pair<std::string, graphics::Texture*>>		s_resources_dynamic_textures;
		static std::vector<std::pair<std::string, graphics::Material*>>		s_resources_dynamic_materials;
		static std::vector<std::pair<std::string, graphics::Model*>>		s_resources_dynamic_models;

	public:


		// Every time we load a resource, we also need to name it, so we can find it easily later..

		static ImageData* load_resource__image(const std::string& path, const std::string& resourceName);
		
		// *If "loadMaterialData" is true
		//	-> we attempt to load all material stuff, including textures, their file paths, etc
		//	from this file. !!!NOTE!!! THIS MAY FAIL, since not all 3d model file formats
		//	store all the material data, we attempt to load here.
		//		-> You need to check after loading this model, does it contain the material data you wanted!
		// 
		//	You may also just set "loadMaterialData" to false and manually set the model's material stuff.
		static graphics::Model* load_resource__model(
			const std::string& path, const std::string& resourceName,
			unsigned int flags, bool loadMaterialData
		);

		static graphics::Texture* create_resource__texture(
			ImageData* imgData, const std::string& resourceName, 
			graphics::TextureFilteringType texFilteringType, graphics::TextureImageMapping texImgMapping
		);

		static graphics::Material* create_resource__material(
			graphics::Texture* diffuseTexture, graphics::Texture* specularTexture, graphics::Texture* normalTexture,
			const std::string& resourceName
		);
		static graphics::Material* create_resource__material(
			const std::vector<graphics::Texture*>& textures,
			const std::string& resourceName
		);

		// Returns pointer to the texture if it was found.
		// Returns nullptr if this resource wasn't found.
		static graphics::Texture* find_texture(const std::string& resourceName);


		// *NOTE Usually all resources allocated by the "client" are considered dynamic by default

		// This can be called manually, but usually this should happen only when the program exits!
		static void free_static_resources();

		// This is called every time scene gets switched!
		static void free_dynamic_resources();

	};
}