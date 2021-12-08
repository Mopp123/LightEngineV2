
#include "Material.h"


namespace lightEngine
{
	namespace graphics
	{
		Material::Material(Texture* diffuseMap, Texture* specularMap, Texture* normalMap) :
			_texture_diffuse(diffuseMap),
			_texture_specular(specularMap),
			_texture_normal(normalMap)
		{
			_allTextures.push_back(_texture_diffuse);
			_allTextures.push_back(_texture_specular);
			_allTextures.push_back(_texture_normal);
		}
		Material::Material(const std::vector<Texture*>& textures)
		{
			_allTextures = textures;
		}

		void Material::setSpecularProperties(float strength, float shininess, const mml::Vec3& color)
		{
			_specular_strength = strength;
			_specular_shininess = shininess;
			_specular_color = color;
		}

		// Adds texture to _allTextures container of this material.
		// Returns index of this texture in the container.
		int Material::addTexture(Texture* texture)
		{
			_allTextures.push_back(texture);
			return _allTextures.size() - 1;
		}

		const Texture* Material::getTexture(const int& index) const
		{
			if (index >= 0 && index < _allTextures.size())
				return _allTextures[index];
			else
				return nullptr;
		}

	}
}