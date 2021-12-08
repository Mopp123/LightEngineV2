#pragma once

#include "graphics\graphicsResources\Mesh.h"
#include "graphics\opengl\OpenglBuffers.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{

			class OpenglMesh : public Mesh
			{
			private:

				unsigned int _vaoID;

			public:

				OpenglMesh(IndexBuffer* indexBuffer, std::vector<VertexBuffer*> vertexBuffers, PrimitiveDrawType drawType);
				~OpenglMesh();

				inline const unsigned int& getVaoID() const { return _vaoID; }
			};
		}
	}
}