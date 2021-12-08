
#include "SkinnedRenderable3D.h"
#include "core\Application.h"

namespace lightEngine
{
	namespace graphics
	{

		SkinnedRenderable3D::SkinnedRenderable3D(Entity* parentEntity, Mesh* mesh) :
			Renderable3D(mesh, ComponentType::RenderableSkinned3D, sizeof(*this))
		{
			_skeleton = mesh->createSkeletonInstance();
			Application::get_scene_manager()->getCurrentScene()->addToScene(_skeleton);
			for(animation::Bone* b : _skeleton->getAllBones())
				Application::get_scene_manager()->getCurrentScene()->addToScene(b);

			parentEntity->addChild(_skeleton);
		}

		SkinnedRenderable3D::~SkinnedRenderable3D()
		{
		}
	}
}