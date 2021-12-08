#pragma once

#include <string>
#include "utils\maths\Maths.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{
			// !!THIS IS FUCKING DUMB!!!

			class OpenglShader;

			class Uniform
			{
			protected:

				OpenglShader* _shader;
				int _location = -1;

			public:

				void init(OpenglShader* shader, const std::string& name);
				virtual ~Uniform();
			};

			class UniformInt : public Uniform
			{
			public:
				void set(const int& v) const;
			};

			class UniformFloat : public Uniform
			{
			public:
				void set(const float& v) const;
			};
			
			class UniformFloat2 : public Uniform
			{
			public:
				void set(const mml::Vec2& v) const;
			};

			class UniformFloat3 : public Uniform
			{
			public:
				void set(const mml::Vec3& v) const;
			};

			class UniformFloat4 : public Uniform
			{
			public:
				void set(const mml::Vec4& v) const;
			};

			class UniformMat4 : public Uniform
			{
			public:
				void set(const mml::Mat4& m) const;
			};
		}
	}
}