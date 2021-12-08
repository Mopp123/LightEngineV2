
#include "OpenglMeshShader.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{


			OpenglMeshShader::OpenglMeshShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) :
				OpenglShader(vertexShaderPath, fragmentShaderPath, OPENGL_SHADER_CREATE__FROM_FILE)
			{}

			OpenglMeshShader::~OpenglMeshShader()
			{}
		}
	}
}