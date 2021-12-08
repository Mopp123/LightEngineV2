
#include "Shader.h"
#include "Graphics.h"

#include "graphics\opengl\openglShaders\shaders3D\OpenglMeshShader.h"
#include "core\Debug.h"

#define OPENGL_VERTEX_SHADER_PATH__MESH_SHADER				"res/shaders/MeshVertexShader.glsl"
#define OPENGL_VERTEX_SHADER_PATH__SIMPLE_MESH_SHADER		"res/shaders/SimpleMeshVertexShader.glsl"
#define OPENGL_VERTEX_SHADER_PATH__SKINNED_MESH_SHADER		"res/shaders/SkinnedMeshVertexShader.glsl"
#define OPENGL_VERTEX_SHADER_PATH__TERRAIN_SHADER			"res/shaders/TerrainVertexShader.glsl"

#define OPENGL_FRAGMENT_SHADER_PATH__MESH_SHADER			"res/shaders/MeshFragmentShader.glsl"
#define OPENGL_FRAGMENT_SHADER_PATH__SIMPLE_MESH_SHADER		"res/shaders/SimpleMeshFragmentShader.glsl"
#define OPENGL_FRAGMENT_SHADER_PATH__SKINNED_MESH_SHADER	"res/shaders/MeshFragmentShader.glsl"
#define OPENGL_FRAGMENT_SHADER_PATH__TERRAIN_SHADER			"res/shaders/TerrainFragmentShader.glsl"



namespace lightEngine
{
	namespace graphics
	{
		// first = vertex shader, second = pixel shader
		static std::pair<std::string, std::string> get_shader_paths__mesh_shader()
		{
			switch (Graphics::get_graphics_API())
			{
			
			case GraphicsAPI::OpenGL: return std::make_pair(
				OPENGL_VERTEX_SHADER_PATH__MESH_SHADER, 
				OPENGL_FRAGMENT_SHADER_PATH__MESH_SHADER
			);

			default:
				break;
			}

			return std::make_pair("ERROR", "ERROR");
		}

		static std::pair<std::string, std::string> get_shader_paths__simple_mesh_shader()
		{
			switch (Graphics::get_graphics_API())
			{

			case GraphicsAPI::OpenGL: return std::make_pair(
				OPENGL_VERTEX_SHADER_PATH__SIMPLE_MESH_SHADER,
				OPENGL_FRAGMENT_SHADER_PATH__SIMPLE_MESH_SHADER
			);

			default:
				break;
			}

			return std::make_pair("ERROR", "ERROR");
		}

		static std::pair<std::string, std::string> get_shader_paths__skinned_mesh_shader()
		{
			switch (Graphics::get_graphics_API())
			{

			case GraphicsAPI::OpenGL: return std::make_pair(
				OPENGL_VERTEX_SHADER_PATH__SKINNED_MESH_SHADER,
				OPENGL_FRAGMENT_SHADER_PATH__SKINNED_MESH_SHADER
			);

			default:
				break;
			}

			return std::make_pair("ERROR", "ERROR");
		}

		static std::pair<std::string, std::string> get_shader_paths__terrain_shader()
		{
			switch (Graphics::get_graphics_API())
			{

			case GraphicsAPI::OpenGL: return std::make_pair(
				OPENGL_VERTEX_SHADER_PATH__TERRAIN_SHADER,
				OPENGL_FRAGMENT_SHADER_PATH__TERRAIN_SHADER
			);

			default:
				break;
			}

			return std::make_pair("ERROR", "ERROR");
		}



		std::map<unsigned int, Shader*> Shader::s_defaultShaders;

		void Shader::create_default_shaders()
		{
			switch (Graphics::get_graphics_API())
			{
			case GraphicsAPI::OpenGL : 
			{
				s_defaultShaders.insert(std::make_pair(DEFAULT_SHADERS__MESH_SHADER_INDEX, new opengl::OpenglShader(get_shader_paths__mesh_shader())));
				s_defaultShaders.insert(std::make_pair(DEFAULT_SHADERS__SIMPLE_MESH_SHADER_INDEX, new opengl::OpenglShader(get_shader_paths__simple_mesh_shader())));
				s_defaultShaders.insert(std::make_pair(DEFAULT_SHADERS__SKINNED_MESH_SHADER_INDEX, new opengl::OpenglShader(get_shader_paths__skinned_mesh_shader())));
				s_defaultShaders.insert(std::make_pair(DEFAULT_SHADERS__TERRAIN_SHADER_INDEX, new opengl::OpenglShader(get_shader_paths__terrain_shader())));
			}break;

			default:
				Debug::log("ERROR: Tried to create default shaders, but invalid graphics API was in use!");
				break;
			}
		}


		Shader* Shader::get_default_mesh_shader()
		{
			return s_defaultShaders[DEFAULT_SHADERS__MESH_SHADER_INDEX];
		}
		Shader* Shader::get_default_simple_mesh_shader()
		{
			return s_defaultShaders[DEFAULT_SHADERS__SIMPLE_MESH_SHADER_INDEX];
		}
		Shader* Shader::get_default_skinned_mesh_shader()
		{
			return s_defaultShaders[DEFAULT_SHADERS__SKINNED_MESH_SHADER_INDEX];
		}
		Shader* Shader::get_default_terrain_shader()
		{
			return s_defaultShaders[DEFAULT_SHADERS__TERRAIN_SHADER_INDEX];
		}
	}
}