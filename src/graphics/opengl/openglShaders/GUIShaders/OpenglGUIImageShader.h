#pragma once

#include "graphics\opengl\openglShaders\OpenglShader.h"
#include "graphics\opengl\openglShaders\Uniforms.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenglGUIImageShader : public OpenglShader
			{
			private:

				UniformMat4 _uniform_projMat;

			public:

				OpenglGUIImageShader();
				~OpenglGUIImageShader();

				void setProjectionMatrix(const mml::Mat4& matrix) const;
			};
		}
	}
}