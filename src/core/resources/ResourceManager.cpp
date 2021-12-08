
#include "ResourceManager.h"
#include "utils\image\ImageLoading.h"
#include "graphics\graphicsUtils\ModelLoading.h"
#include "core\Debug.h"
#include "core\Common.h"


namespace lightEngine
{
	std::vector<std::pair<std::string, ImageData*>>				ResourceManager::s_resources_dynamic_images;
	std::vector<std::pair<std::string, graphics::Texture*>>		ResourceManager::s_resources_dynamic_textures;
	std::vector<std::pair<std::string, graphics::Material*>>	ResourceManager::s_resources_dynamic_materials;
	std::vector<std::pair<std::string, graphics::Model*>>		ResourceManager::s_resources_dynamic_models;



	// Every time we load a resource, we also need to name it, so we can find it easily later..

	ImageData* ResourceManager::load_resource__image(const std::string& path, const std::string& resourceName)
	{
		ImageData* imgDat = imageLoading::load_image(path);
		s_resources_dynamic_images.push_back(std::make_pair(resourceName, imgDat));
		return imgDat;
	}

	
	// *If "loadMaterialData" is true
	//	-> we attempt to load all material stuff, including textures, their file paths, etc
	//	from this file. !!!NOTE!!! THIS MAY FAIL, since not all 3d model file formats
	//	store all the material data, we attempt to load here.
	//		-> You need to check after loading this model, does it contain the material data you wanted!
	// 
	//	You may also just set "loadMaterialData" to false and manually set the model's material stuff.
	graphics::Model* ResourceManager::load_resource__model(
		const std::string& path, const std::string& resourceName,
		unsigned int flags, bool loadMaterialData
	)
	{
		graphics::Model* model = graphics::modelLoading::load_model(path, flags, loadMaterialData);
		s_resources_dynamic_models.push_back(std::make_pair(resourceName, model));
		return model;
	}

	graphics::Texture* ResourceManager::create_resource__texture(
		ImageData* imgData, const std::string& resourceName,
		graphics::TextureFilteringType texFilteringType, graphics::TextureImageMapping  texImgMapping
	)
	{
		graphics::Texture* texture = graphics::Texture::create(imgData, texFilteringType, texImgMapping);
		s_resources_dynamic_textures.push_back(std::make_pair(resourceName, texture));
		return texture;
	}

	graphics::Material* ResourceManager::create_resource__material(
		graphics::Texture* diffuseTexture, graphics::Texture* specularTexture, graphics::Texture* normalTexture,
		const std::string& resourceName
	)
	{
		graphics::Material* material = new graphics::Material(diffuseTexture, specularTexture, normalTexture);
		s_resources_dynamic_materials.push_back(std::make_pair(resourceName, material));
		return material;
	}
	graphics::Material* ResourceManager::create_resource__material(
		const std::vector<graphics::Texture*>& textures,
		const std::string& resourceName
	)
	{
		graphics::Material* material = new graphics::Material(textures);
		s_resources_dynamic_materials.push_back(std::make_pair(resourceName, material));
		return material;
	}


	graphics::Texture* ResourceManager::find_texture(const std::string& resourceName)
	{
		for (std::pair<std::string, graphics::Texture*>& resource : s_resources_dynamic_textures)
		{
			if (resource.first == resourceName)
				return resource.second;
		}
		return nullptr;
	}



	// *NOTE Usually all resources allocated by the "client" are considered dynamic by default

	// This can be called manually, but usually this should happen only when the program exits!
	void ResourceManager::free_static_resources()
	{
		Debug::log("All static resources freed!");
	}



	template<typename T>
	static void destroy_resource_container(std::vector<std::pair<std::string, T*>>& container)
	{
		for (std::pair<std::string, T*>& resource : container)
		{
			delete resource.second;
			resource.second = nullptr;
		}
		container.clear();
	}

	// This is called every time scene gets switched!
	void ResourceManager::free_dynamic_resources()
	{
		Debug::log("All dynamic resources freed!");

		// Regular dynamic resources
		destroy_resource_container<ImageData>(s_resources_dynamic_images);
		destroy_resource_container<graphics::Material>(s_resources_dynamic_materials);
		destroy_resource_container<graphics::Texture>(s_resources_dynamic_textures);
		destroy_resource_container<graphics::Model>(s_resources_dynamic_models);
	}
}