
#include "Mesh.h"
#include "graphics\Graphics.h"

#include "graphics\opengl\openglGraphicsResources\OpenglMesh.h"
#include "animations\skeletalAnimation\Skeleton.h"

#include "core\Debug.h"

namespace lightEngine
{
	namespace graphics
	{
		Mesh::Mesh(PrimitiveDrawType drawType) :
			_drawType(drawType)
		{}

		Mesh* Mesh::create(IndexBuffer* indexBuffer, std::vector<VertexBuffer*> vertexBuffers, PrimitiveDrawType drawType)
		{
			switch (Graphics::get_graphics_API())
			{
			case GraphicsAPI::OpenGL: return new opengl::OpenglMesh(indexBuffer, vertexBuffers, drawType);
			}
		}


		Mesh::~Mesh()
		{
			delete _indexBuffer;
			for (VertexBuffer* vb : _vertexBuffers)
				delete vb;

			// If this mesh had skeleton prototype -> destroy it(and its' bones) since it was never 
			// added to the scene (which deletes all its' entities when destroyed)
			if (_skeleton)
			{
				for (animation::Bone* b : _skeleton->getAllBones())
				{
					delete b;
					b = nullptr;
				}
			}
			delete _skeleton;
			_skeleton = nullptr;
		}


		// Creates instance of a skeleton (if this mesh was a skinned mesh)
		animation::Skeleton* Mesh::createSkeletonInstance()
		{
			return new animation::Skeleton(_skeleton);
		}
	}
}