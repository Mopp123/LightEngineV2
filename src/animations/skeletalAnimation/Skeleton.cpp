
#include "Skeleton.h"
#include "entities\Entity.h"
#include "core\Debug.h"

namespace lightEngine
{
	namespace animation
	{
		Skeleton::Skeleton()
		{
		}
		Skeleton::~Skeleton()
		{}

		void Skeleton::setPose(int keyframe)
		{
			for (Bone* b : _bones)
				b->setPose(keyframe);
		}

		void Skeleton::interpolatePoses(unsigned int keyframeIndex0, unsigned int keyframeIndex1, float amount)
		{
			for (Bone* b : _bones)
				b->interpolatePoses(keyframeIndex0, keyframeIndex1, amount);
		}

		void Skeleton::loopAnimation(unsigned int animIndex) 
		{
#ifdef LIGHT_ENGINE__DEBUG_MODE__FULL
			if (animIndex >= _animations.size())
			{
				Debug::log("ERROR: Location: Skeleton::loopAnimation(unsigned int)\nanimIndex was out of bounds!");
				return;
			}
#endif
			Animation& anim = _animations[animIndex];
			anim.loop();
			interpolatePoses(anim._currentKeyframeIndex, anim._nextKeyframeIndex, anim._interpolationAmount);
		}

		void Skeleton::playAnimation(unsigned int animIndex) 
		{
#ifdef LIGHT_ENGINE__DEBUG_MODE__FULL
			if (animIndex >= _animations.size())
			{
				Debug::log("ERROR: Location: Skeleton::playAnimation(unsigned int)\nanimIndex was out of bounds!");
				return;
			}
#endif
			Animation& anim = _animations[animIndex];
			anim.play();
			interpolatePoses(anim._currentKeyframeIndex, anim._nextKeyframeIndex, anim._interpolationAmount);
		}
		
		Skeleton::Skeleton(Skeleton* prototype)
		{
			_boneMapping = prototype->_boneMapping;
			int rootBoneIndex = 0;
			_boneCount = prototype->_boneCount;
			for (Bone* bone : prototype->_bones)
			{
				if (prototype->_rootBone == bone)
					rootBoneIndex = _boneMapping[bone->_name];

				Bone* newBone = new Bone(*bone);
				newBone->_skeleton = this;
				
				_bones.push_back(newBone);
			}

			setRootBone(_bones[rootBoneIndex]);

			for (Bone* b : _bones)
			{
				if (b->_parentBoneIndex > -1)
				{
					b->getParentBone()->addChild(b);
				}
			}
		}

		void Skeleton::setBones(std::vector<Bone*>& bones)
		{
			_bones = bones;
		}

		void Skeleton::setRootBone(Bone* bone)
		{
			_rootBone = bone;
			addChild(_rootBone);
		}

		void Skeleton::addBone(unsigned int parentBoneIndex, unsigned int childBoneIndex)
		{
			Bone* parent = _bones[parentBoneIndex];
			Bone* child = _bones[childBoneIndex];
			child->_skeleton = this;
			child->_parentBoneIndex = parentBoneIndex;
			parent->addChild(child);
			_boneCount++;
		}

		std::vector<mml::Mat4> Skeleton::getAllBoneMatrices() const
		{
			std::vector<mml::Mat4> matrices;
			matrices.reserve(_bones.size());

			for (const Bone* b : _bones)
				matrices.emplace_back(b->createFinalBoneMatrix());


			return matrices;
		}
	}
}