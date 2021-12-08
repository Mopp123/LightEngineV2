
#include "OpenglGUIImageShader.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{

			static std::string get_default_vertex_shader_src()
			{
				return R"(
					#version 330 core
					layout(location = 0) in vec2 position;

					layout(location = 1) in vec4 transform; // x,y = position : z,w = scale
					layout(location = 2) in vec4 color;

					uniform mat4 projectionMatrix;

					out vec4 passColor;
					out vec2 passTexCoord;
					
					void main(void)
					{
						vec2 transform_pos = vec2(transform.x, transform.y);
						vec2 transform_scale = vec2(transform.z, transform.w);
					

						gl_Position = vec4(position * transform_scale + transform_pos, 0, 1.0);

						passColor = color;
						passTexCoord = (vec2((position.x + 1.0) / 2.0, (position.y + 1.0) / 2.0) + vec2(0,0)) / 1;

					}
				)";
			}

			static std::string get_default_fragment_shader_src()
			{
				return R"(
					#version 330 core
					
					in vec4 passColor;
					in vec2 passTexCoord;

					uniform sampler2D textureSampler;

					out vec4 outColor;
					
					void main(void)
					{
						outColor = texture(textureSampler, passTexCoord) * passColor;
					}
				)";
			}


			OpenglGUIImageShader::OpenglGUIImageShader() :
				OpenglShader(get_default_vertex_shader_src(), get_default_fragment_shader_src(), OPENGL_SHADER_CREATE__FROM_SOURCE)
			{
				_uniform_projMat.init(this, "projectionMatrix");
			}

			OpenglGUIImageShader::~OpenglGUIImageShader()
			{}


			void OpenglGUIImageShader::setProjectionMatrix(const mml::Mat4& matrix) const
			{
				_uniform_projMat.set(matrix);
			}
		}
	}
}