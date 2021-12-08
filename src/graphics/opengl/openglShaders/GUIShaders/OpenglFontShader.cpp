
#include "OpenglFontShader.h"

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
					
					layout(location = 1) in vec4 transform;// xy = position, zw = scale
					layout(location = 2) in vec2 texOffset;
					layout(location = 3) in vec4 color;
					layout(location = 4) in float z;
					
					uniform float textureAtlasRowCount;
					uniform mat4 projectionMatrix;

					out vec2 var_texCoord;
					out vec4 var_passColor;
					
					void main(void)
					{
						gl_Position = projectionMatrix * vec4((position * transform.zw) + transform.xy, z, 1.0);
						
						var_texCoord = (vec2((position.x + 1.0) / 2.0, 1.0 - (position.y + 1.0) / 2.0) + texOffset) / textureAtlasRowCount;
						var_passColor = color;
					}
				)";
			}

			static std::string get_default_fragment_shader_src()
			{
				return R"(
					#version 330 core

					in vec2 var_texCoord;
					in vec4 var_passColor;
					
					uniform sampler2D textureColor;
					
					out vec4 fragColor;

					const float width = 0.5f;
					const float edge = 0.1f;

					const float borderWidth = 0.5f;
					const float borderEdge = 0.4f;

					
					void main(void)
					{
						vec3 outlineColor = vec3(0,0,0);

						vec4 textureColor = texture(textureColor, var_texCoord);
						float textureIntensity = textureColor.r;
						//fragColor = vec4(var_passColor.r, var_passColor.g, var_passColor.b, textureIntensity * var_passColor.a);
						
						float texAlpha = textureIntensity;

						float distance = 1.0f - texAlpha;
						float alpha = 1.0f - smoothstep(width, width + edge, distance);
						float outlineAlpha = 1.0f - smoothstep(borderWidth, borderWidth + borderEdge, distance);
						float totalAlpha = alpha + (1.0f - alpha) * outlineAlpha;

						vec3 totalColor = mix(outlineColor, var_passColor.rgb, alpha / totalAlpha);
						
						// PROBLEM : No more than one letter per text can be rendered if we don't force alpha 
						// to 0 or discard on small alpha values here, because all letters of the same text obj
						// are on the same z position (or layer) !!!
						if(totalAlpha < 0.1)
							discard;

						fragColor = vec4(totalColor, totalAlpha);
					}
				)";
			}

			OpenglFontShader::OpenglFontShader() :
				OpenglShader(get_default_vertex_shader_src(), get_default_fragment_shader_src(), OPENGL_SHADER_CREATE__FROM_SOURCE)
			{
				_uniform_projMat.init(this, "projectionMatrix");
				_uniform_texAtlasRows.init(this, "textureAtlasRowCount");
			}

			OpenglFontShader::~OpenglFontShader()
			{}


			void OpenglFontShader::setProjectionMatrix(const mml::Mat4& matrix) const
			{
				_uniform_projMat.set(matrix);
			}
			void OpenglFontShader::setTextureAtlasRowCount(const float& rows) const
			{
				_uniform_texAtlasRows.set(rows);
			}

		}
	}
}