#pragma once

#include "entities\Entity.h"
#include "animations\skeletalAnimation\Keyframe.h"
#include <string>

namespace lightEngine
{
	namespace animation
	{
		class Skeleton;

		class Bone : public Entity
		{
		private:

			mml::Mat4 _inverseBindPoseTransformationMatrix;
			int _parentBoneIndex = -1;
			Skeleton* _skeleton = nullptr;

			std::vector<Keyframe> _keyframes;

			friend class Skeleton;

		public:

			Bone();
			Bone(const std::string& name);
			Bone(const Bone& bone);

			Bone* getParentBone();

			void setPose(unsigned int keyframeIndex);
			void interpolatePoses(unsigned int keyframeIndex0, unsigned int keyframeIndex1, float amount);

			void addKeyframe(const Keyframe& keyframe);

			mml::Mat4 createFinalBoneMatrix() const;

			inline void setInverseBindPoseMatrix(const mml::Mat4& matrix) { _inverseBindPoseTransformationMatrix = matrix; }
			inline mml::Mat4& getInverseBindPoseMatrix() { return _inverseBindPoseTransformationMatrix; }

			inline const std::vector<Keyframe>& getKeyframes() const { return _keyframes; }
		};
	}
}