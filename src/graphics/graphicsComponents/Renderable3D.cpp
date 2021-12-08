
#include "Renderable3D.h"


namespace lightEngine
{
	namespace graphics
	{

		Renderable3D::Renderable3D(Mesh* mesh) : 
			Component(ComponentType::Renderable3D, sizeof(*this))
		{
			_mesh = mesh;
		}

		Renderable3D::Renderable3D(Mesh* mesh, ComponentType extendedComponentType, size_t extendedComponentSize) :
			Component(extendedComponentType, extendedComponentSize)
		{
			_mesh = mesh;
		}

		Renderable3D::~Renderable3D()
		{}
	}
}