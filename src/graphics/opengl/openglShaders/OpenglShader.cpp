
#include <glew.h>
#include "OpenglShader.h"
#include "core\Application.h"
#include "core\Debug.h"

#include <vector>

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{
			static std::string load_shader_source(const std::string& path)
			{
				FILE* file;
				fopen_s(&file, path.c_str(), "rb");
				if (!file)
				{
					Debug::log("ERROR: Failed to load shader from " + path);
					fclose(file);
					return "ERROR";
				}

				fseek(file, 0, SEEK_END);
				unsigned int fileLength = ftell(file) + 1;
				fseek(file, 0, SEEK_SET);

				char* rawSource = new char[fileLength];
				memset(rawSource, 0, fileLength);

				if (fread_s(rawSource, fileLength, 1, fileLength, file) != fileLength - 1)
				{
					Debug::log("ERROR: Failed to read shader from " + path);
					fclose(file);
					return "ERROR";
				}

				fclose(file);

				std::string source(rawSource);
				delete[] rawSource;

				return source;
			}

			OpenglShader::OpenglShader(const std::string& vertex, const std::string& fragment, unsigned char creationType)
			{
				std::string vertexShaderSource = vertex;
				std::string fragmentShaderSource = fragment;

				if (creationType == OPENGL_SHADER_CREATE__FROM_FILE)
				{
					vertexShaderSource = load_shader_source(vertex);
					fragmentShaderSource = load_shader_source(fragment);
				}

				_vertexShaderID = createShader(vertexShaderSource, OPENGL_SHADER_TYPE__VERTEX_SHADER);
				_fragmentShaderID = createShader(fragmentShaderSource, OPENGL_SHADER_TYPE__FRAGMENT_SHADER);

				_programID = glCreateProgram();
				glAttachShader(_programID, _vertexShaderID);
				glAttachShader(_programID, _fragmentShaderID);
				
				glLinkProgram(_programID);
				glValidateProgram(_programID);
			}

			OpenglShader::OpenglShader(const std::pair<std::string, std::string>& shaderPaths) :
				OpenglShader(shaderPaths.first, shaderPaths.second, OPENGL_SHADER_CREATE__FROM_FILE)
			{}


			OpenglShader::~OpenglShader()
			{
				unbind();
				glDetachShader(_programID, _vertexShaderID);
				glDetachShader(_programID, _fragmentShaderID);

				glDeleteShader(_vertexShaderID);
				glDeleteShader(_fragmentShaderID);

				glDeleteProgram(_programID);
			}


			void OpenglShader::bind()
			{
				glCall(glUseProgram(_programID));
			}

			void OpenglShader::unbind()
			{
				glUseProgram(0);
			}


			void OpenglShader::setUniform_Int(int location, const int& val) const
			{
				glUniform1i(location, val);
			}
			void OpenglShader::setUniform_Float(int location, const float& val) const
			{
				glUniform1f(location, val);
			}
			void OpenglShader::setUniform_Float2(int location, const mml::Vec2& vec) const 
			{
				glUniform2f(location, vec.x, vec.y);
			}
			void OpenglShader::setUniform_Float3(int location, const mml::Vec3& vec) const
			{
				glUniform3f(location, vec.x, vec.y, vec.z);
			}
			void OpenglShader::setUniform_Float4(int location, const mml::Vec4& vec) const
			{
				glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
			}
			void OpenglShader::setUniform_Mat4(int location, const mml::Mat4& mat) const
			{
				glCall(glUniformMatrix4fv(location, 1, false, &mat[0]));
			}

			void OpenglShader::setUniform_Int(const std::string& uniformName, const int& val) const
			{
				setUniform_Int(getUniformLocation(uniformName), val);
			}
			void OpenglShader::setUniform_Float(const std::string& uniformName, const float& val) const
			{
				setUniform_Float(getUniformLocation(uniformName), val);
			}
			void OpenglShader::setUniform_Float2(const std::string& uniformName, const mml::Vec2& vec) const
			{
				setUniform_Float2(getUniformLocation(uniformName), vec);
			}
			void OpenglShader::setUniform_Float3(const std::string& uniformName, const mml::Vec3& vec) const
			{
				setUniform_Float3(getUniformLocation(uniformName), vec);
			}
			void OpenglShader::setUniform_Float4(const std::string& uniformName, const mml::Vec4& vec) const
			{
				setUniform_Float4(getUniformLocation(uniformName), vec);
			}
			void OpenglShader::setUniform_Mat4(const std::string& uniformName, const mml::Mat4& mat) const
			{
				setUniform_Mat4(getUniformLocation(uniformName), mat);
			}





			int OpenglShader::getUniformLocation(const std::string& uniformName) const
			{
				if (_locationCache.find(uniformName) != _locationCache.end())
					return _locationCache[uniformName];

				int location = glCall(glGetUniformLocation(_programID, uniformName.c_str()));
				_locationCache.insert(std::make_pair(uniformName, location));
				return location;
			}


			// shaderType can be either GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
			unsigned int OpenglShader::createShader(const std::string& shaderSource, unsigned int shaderType)
			{
				GLenum glShaderType = GL_NONE;
				switch (shaderType)
				{
				case OPENGL_SHADER_TYPE__VERTEX_SHADER: glShaderType = GL_VERTEX_SHADER;
					break;
				case OPENGL_SHADER_TYPE__FRAGMENT_SHADER: glShaderType = GL_FRAGMENT_SHADER;
					break;
				
				default:
					break;
				}
				if (glShaderType == GL_NONE)
				{
					Debug::log("ERROR: Location: OpenglShader::createShader(const std::string&, unsigned int)\nInvalid shader type!");
					return 0;
				}

				unsigned int shaderID = glCreateShader(glShaderType);

				const char* const finalShaderSource = shaderSource.c_str();
				glShaderSource(shaderID, 1, &finalShaderSource, 0);
				glCompileShader(shaderID);

				GLint compileResult = -1;
				GLint infoLogLength = -1;

				glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileResult);
				if (compileResult == GL_FALSE)
				{
					glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
				
					std::vector<char> infoLog(infoLogLength);
					glGetShaderInfoLog(shaderID, sizeof(char) * infoLog.size(), &infoLogLength, &infoLog[0]);

					std::string shaderTypeString = "ERROR";
					switch (shaderType)
					{
					case OPENGL_SHADER_TYPE__VERTEX_SHADER:		shaderTypeString = "Vertex"; break;
					case OPENGL_SHADER_TYPE__FRAGMENT_SHADER:	shaderTypeString = "Fragment"; break;
					default: 
						break;
					}
					Debug::log("ERROR: Failed to compile" + shaderTypeString + " shader!");
					
					for(char c : infoLog)
						printf("%c", c);
				}

				return shaderID;
			}
		}
	}
}