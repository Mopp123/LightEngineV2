#pragma once

#include <map>
#include <string>


#define DEFAULT_SHADERS__MESH_SHADER_INDEX			0
#define DEFAULT_SHADERS__SIMPLE_MESH_SHADER_INDEX	1
#define DEFAULT_SHADERS__SKINNED_MESH_SHADER_INDEX	2
#define DEFAULT_SHADERS__TERRAIN_SHADER_INDEX		3

namespace lightEngine
{
	namespace graphics
	{

		class Shader
		{
		private:

			static std::map<unsigned int, Shader*> s_defaultShaders;

		public:

			virtual ~Shader() {}

			virtual void bind() = 0;
			virtual void unbind() = 0;

			static void create_default_shaders();

			static Shader* get_default_mesh_shader();
			static Shader* get_default_simple_mesh_shader();
			static Shader* get_default_skinned_mesh_shader();
			static Shader* get_default_terrain_shader();

		};
	}
}