#pragma once

#include "Texture.h"
#include "graphics\Shader.h"
#include "core\Common.h"
#include <vector>

namespace lightEngine
{
	namespace graphics
	{
		class Material
		{
		private:

			std::vector<Texture*> _allTextures;

			Texture* _texture_diffuse = nullptr;
			Texture* _texture_specular = nullptr;
			Texture* _texture_normal = nullptr;

			float _specular_strength = 0.5f;
			float _specular_shininess = 16.0f;
			mml::Vec3 _specular_color = mml::Vec3(1, 1, 1);

			// Which face to cull.
			// Possible values : 
			//	* LE__GRAPHICS__MESH_BACK_FACE(default)
			//	* LE__GRAPHICS__MESH_FRONT_FACE
			//	* 0 (don't cull faces)
			byte _culledFace = LE__GRAPHICS__MESH_BACK_FACE;

			Shader* _shader = nullptr;

		public:

			// *NOTE Material doesn't own its' textures!
			Material(Texture* diffuseMap = nullptr, Texture* specularMap = nullptr, Texture* normalMap = nullptr);
			Material(const std::vector<Texture*>& textures);

			void setSpecularProperties(float strength, float shininess, const mml::Vec3& color);

			// Adds texture to _allTextures container of this material.
			// Returns index of this texture in the container.
			int addTexture(Texture* texture);
			const Texture* getTexture(const int& index) const;

			inline void setShader(Shader* shader) { _shader = shader; }

			inline const Texture* getTexture_diffuse() const { return _texture_diffuse; }
			inline const Texture* getTexture_specular() const { return _texture_specular; }
			inline const Texture* getTexture_normal() const { return _texture_normal; }
			inline const std::vector<Texture*>& getAllTextures() const { return _allTextures; }

			inline const float& getSpecular_strength() const { return _specular_strength; }
			inline const float& getSpecular_shininess() const { return _specular_shininess; }
			inline const mml::Vec3& getSpecular_color() const { return _specular_color; }

			inline const byte getCulledFace() const { return _culledFace; }

			inline Shader* getShader() { return _shader; }
		};
	}
}