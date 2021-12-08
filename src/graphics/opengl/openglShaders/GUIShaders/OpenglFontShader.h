#pragma once

#include "graphics\opengl\openglShaders\OpenglShader.h"
#include "graphics\opengl\openglShaders\Uniforms.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenglFontShader : public OpenglShader
			{
			private:

				UniformMat4 _uniform_projMat;
				UniformFloat _uniform_texAtlasRows; // texture atlas' row count

			public:

				OpenglFontShader();
				~OpenglFontShader();

				void setProjectionMatrix(const mml::Mat4& matrix) const;
				void setTextureAtlasRowCount(const float& rows) const;

			};
		}
	}
}