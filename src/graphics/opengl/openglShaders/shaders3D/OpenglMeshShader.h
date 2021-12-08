#pragma once

#include "graphics\opengl\openglShaders\OpenglShader.h"
#include "graphics\opengl\openglShaders\Uniforms.h"
#include "graphics\graphicsResources\Material.h"
#include "graphics\graphicsComponents\lights\DirectionalLight.h"
#include "utils\maths\Maths.h"


namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenglMeshShader : public OpenglShader
			{
			private:

			public:

				OpenglMeshShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
				~OpenglMeshShader();
			};
		}
	}
}