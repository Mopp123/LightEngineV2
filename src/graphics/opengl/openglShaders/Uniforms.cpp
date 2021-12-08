
#include "Uniforms.h"
#include "OpenglShader.h"
#include "core\Application.h"
#include "core\Debug.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{
			void Uniform::init(OpenglShader* shader, const std::string& name)
			{
				_shader = shader;
#ifdef LIGHT_ENGINE__DEBUG_MODE__FULL
				if (!_shader)
				{
					Debug::log(
						"ERROR: Location: Uniform::Uniform(OpenglShader*, const std::string&)\n"
						"Tried to create uniform '" + name + "' but specified shader was nullptr!"
					);
					Application::exit();
					return;
				}
#endif

				_location = _shader->getUniformLocation(name);
			}

			Uniform::~Uniform()
			{}

			
			void UniformInt::set(const int& v) const
			{
				_shader->setUniform_Int(_location, v);
			}
			void UniformFloat::set(const float& v) const
			{
				_shader->setUniform_Float(_location, v);
			}
			void UniformFloat2::set(const mml::Vec2& v) const
			{
				_shader->setUniform_Float2(_location, v);
			}
			void UniformFloat3::set(const mml::Vec3& v) const
			{
				_shader->setUniform_Float3(_location, v);
			}
			void UniformFloat4::set(const mml::Vec4& v) const
			{
				_shader->setUniform_Float4(_location, v);
			}
			void UniformMat4::set(const mml::Mat4& m) const
			{
				_shader->setUniform_Mat4(_location, m);
			}
		}
	}
}