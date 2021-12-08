#pragma once

#include "Bone.h"
#include "Animation.h"

namespace lightEngine
{
	class Entity;

	namespace animation
	{
		class Skeleton : public Entity
		{
		private:

			Bone* _rootBone = nullptr;
			std::vector<Bone*> _bones;
			std::map<std::string, unsigned int> _boneMapping; // second means the index in the Entity::_children

			std::vector<Animation> _animations;

			unsigned int _boneCount = 0;

			friend class Bone;

		public:

			// *NOTE Skeleton doesn't own its' bones!
			// Skeleton and its' bones are "Entities" that are owned by the current "Scene" just like normal entities
			Skeleton();
			Skeleton(Skeleton* prototype);
			~Skeleton();

			void setPose(int keyframe);
			void interpolatePoses(unsigned int keyframeIndex0, unsigned int keyframeIndex1, float amount);
			void loopAnimation(unsigned int animIndex);
			void playAnimation(unsigned int animIndex);

			void setBones(std::vector<Bone*>& bones);
			void setRootBone(Bone* bone);
			void addBone(unsigned int parentBoneIndex, unsigned int childBoneIndex);
			void setBoneMapping(const std::map<std::string, unsigned int>& mapping) { _boneMapping = mapping; }

			inline void addAnimation(const Animation& animation) { _animations.push_back(animation); }
			inline void setAnimations(const std::vector<Animation>& anims) { _animations = anims; }

			std::vector<mml::Mat4> getAllBoneMatrices() const;

			inline const unsigned int getBoneCount() const { return _boneCount; }
			inline std::vector<Bone*>& getAllBones() { return _bones; }

			inline std::vector<Animation>& getAllAnimations() { return _animations; }
		};
	}
}