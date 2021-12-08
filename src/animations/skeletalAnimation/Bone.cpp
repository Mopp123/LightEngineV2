
#include "Bone.h"
#include "Skeleton.h"
#include "core\Debug.h"

namespace lightEngine
{
	namespace animation
	{
		Bone::Bone(const std::string& name) : 
			Entity(name)
		{}
		
		Bone::Bone() : 
			Entity("Bone")
		{
		}
		Bone::Bone(const Bone& bone)
		{
			_inverseBindPoseTransformationMatrix = bone._inverseBindPoseTransformationMatrix;
			_parentBoneIndex = bone._parentBoneIndex;

			for(const Keyframe& keyframe : bone._keyframes)
				_keyframes.push_back({ keyframe.getPosition(), keyframe.getRotation(), keyframe.getScale() });

			_children.clear();

			_skeleton = nullptr;
		}


		Bone* Bone::getParentBone()
		{
			if (!_skeleton)
				return nullptr;

			if (_parentBoneIndex > _skeleton->_bones.size())
				return nullptr;

			return _skeleton->_bones[_parentBoneIndex];
		}

		void Bone::setPose(unsigned int keyframeIndex)
		{
#ifdef LIGHT_ENGINE__DEBUG_MODE__FULL
			if (keyframeIndex >= _keyframes.size())
			{
				Debug::log("ERROR: Location: Bone::setPose(int keyframeIndex)\nKeyframe index was out of bounds!");
				return;
			}
#endif
			Keyframe& currentFrame = _keyframes[keyframeIndex];
			_transform.setPosition(currentFrame.getPosition());
			_transform.setRotation(currentFrame.getRotation());
			_transform.setScale(currentFrame.getScale());
		}

		void Bone::interpolatePoses(unsigned int keyframeIndex0, unsigned int keyframeIndex1, float amount)
		{
			const Keyframe& frame0 = _keyframes[keyframeIndex0];
			const Keyframe& frame1 = _keyframes[keyframeIndex1];

			mml::Vec3 resultPos = mml::lerp(frame0.getPosition(), frame1.getPosition(), amount);
			mml::Quat resultRot = mml::slerp(frame0.getRotation(), frame1.getRotation(), amount);
			mml::Vec3 resultScale = mml::lerp(frame0.getScale(), frame1.getScale(), amount);

			_transform.setPosition(resultPos);
			_transform.setRotation(resultRot);
			_transform.setScale(resultScale);
		}

		void Bone::addKeyframe(const Keyframe& keyframe)
		{
			/*printf("added keyframe : %.2f,%.2f,%.2f   %.2f,%.2f,%.2f,%.2f\n",
				keyframe.getPosition().x, keyframe.getPosition().y, keyframe.getPosition().z,
				keyframe.getRotation().x, keyframe.getRotation().y, keyframe.getRotation().z, keyframe.getRotation().w);*/
			_keyframes.push_back(keyframe);
		}


		mml::Mat4 Bone::createFinalBoneMatrix() const
		{
			return _transform.getTransformationMatrix() * _inverseBindPoseTransformationMatrix;
		}
	}
}