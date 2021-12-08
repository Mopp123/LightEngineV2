#pragma once

#include "Renderable3D.h"
#include "animations\skeletalAnimation\Skeleton.h"

namespace lightEngine
{

	namespace graphics
	{
		class SkinnedRenderable3D : public Renderable3D
		{
		private:

			animation::Skeleton* _skeleton = nullptr;

		public:

			/*
				Currently we need to pass in the parent entity, so we can give this renderable's
				"skeleton entity instance" as a child to the parent entity..
				..quite confusing and shit..
			*/
			SkinnedRenderable3D(Entity* parentEntity, Mesh* mesh);
			~SkinnedRenderable3D();

			inline void setPose(int keyframeIndex) { _skeleton->setPose(keyframeIndex); }
			inline const animation::Skeleton* getSkeleton() const { return _skeleton; }
			inline animation::Skeleton* getSkeleton() { return _skeleton; }
		};
	}
}