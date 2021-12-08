#pragma once

#include "graphics\Shader.h"
#include "utils\maths\Maths.h"
#include <map>

// this is fucking stupid.. but so we dont always mix GLenums and unsigned ints for safety reasons..
#define OPENGL_SHADER_TYPE__VERTEX_SHADER	0
#define OPENGL_SHADER_TYPE__FRAGMENT_SHADER 1

#define OPENGL_SHADER_CREATE__FROM_SOURCE 0
#define OPENGL_SHADER_CREATE__FROM_FILE 1


namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenglShader : public Shader
			{
			private:

				unsigned int _vertexShaderID;
				unsigned int _fragmentShaderID;
				unsigned int _programID;

				mutable std::map<std::string, int> _locationCache;

				friend class Uniform;
				friend class UniformInt;
				friend class UniformFloat;
				friend class UniformFloat2;
				friend class UniformFloat3;
				friend class UniformFloat4;
				friend class UniformMat4;

			public:

				// if creationType == OPENGL_SHADER_CREATE__FROM_SOURCE
				//		-> "vertex" and "fragment" are interpreted as source codes of these shaders.
				// if creationType == OPENGL_SHADER_CREATE__FROM_FILE
				//		-> "vertex" and "fragment" are interpreted as file paths to these shaders.
				OpenglShader(const std::string& vertex, const std::string& fragment, unsigned char creationType);
				// This concideres "shaderPaths".first to be vertex shader and "shaderPaths".second to be fragment shader
				OpenglShader(const std::pair<std::string, std::string>& shaderPaths);

				virtual ~OpenglShader();

				virtual void bind() override;
				virtual void unbind() override;

			public:

				// shaderType can be either OPENGL_SHADER_TYPE__VERTEX_SHADER or OPENGL_SHADER_TYPE__FRAGMENT_SHADER
				unsigned int createShader(const std::string& shaderSource, unsigned int shaderType);


				void setUniform_Int(int location, const int& val) const;
				void setUniform_Float(int location, const float& val) const;
				void setUniform_Float2(int location, const mml::Vec2& vec) const;
				void setUniform_Float3(int location, const mml::Vec3& vec) const;
				void setUniform_Float4(int location, const mml::Vec4& vec) const;
				void setUniform_Mat4(int location, const mml::Mat4& mat) const;

				void setUniform_Int(const std::string& uniformName, const int& val) const;
				void setUniform_Float(const std::string& uniformName, const float& val) const;
				void setUniform_Float2(const std::string& uniformName, const mml::Vec2& vec) const;
				void setUniform_Float3(const std::string& uniformName, const mml::Vec3& vec) const;
				void setUniform_Float4(const std::string& uniformName, const mml::Vec4& vec) const;
				void setUniform_Mat4(const std::string& uniformName, const mml::Mat4& mat) const;

				int getUniformLocation(const std::string& uniformName) const;

			};
		}
	}
}