#include "bmpch.h"

#include "Animation.h"



namespace Basement
{

	Animation::Animation(const std::string& path, SharedPtr<Model> model)
	{
		Assimp::Importer importer;

		auto scene = importer.ReadFile(path, aiProcess_Triangulate);
		BM_CORE_ASSERT(scene->mRootNode, "Scene root node is nullptr");

		auto animation = scene->mAnimations[0];
		m_Duration = static_cast<float>(animation->mDuration);
		m_TicksPerSecond = static_cast<int>(animation->mTicksPerSecond);

		BM_CORE_INFO("Load animation from: {0} ticksPerSecond: {1}, duration: {2}", path, m_TicksPerSecond, m_Duration);

		m_RootNode = ReadHierarchyData(scene->mRootNode);

		ReadMissingBones(animation, *model);
	}

	SharedPtr<Bone> Animation::FindBone(const std::string& name)
	{
		for (auto itr = m_Bones.cbegin(); itr != m_Bones.cend(); ++itr)
		{
			auto bone = *itr;
			if (bone->GetName() == name)
			{
				return bone;
			}
		}

		return nullptr;
	}

	SharedPtr<BoneNode> Animation::ReadHierarchyData(const aiNode* source)
	{
		auto result = std::make_shared<BoneNode>();
		BM_CORE_ASSERT(source, "Invalid source.");

		result->name = source->mName.data;
		result->localTransformation = Vqs(source->mTransformation);

		for (unsigned int i = 0; i < source->mNumChildren; i++)
		{
			SharedPtr<BoneNode> child = ReadHierarchyData(source->mChildren[i]);
			result->children.push_back(child);
		}

		return result;
	}

	void Animation::ReadMissingBones(const aiAnimation* animation, Model& model)
	{
		auto boneInfoMap = model.GetBoneMap();
		auto boneCount = static_cast<int>(boneInfoMap.size());

		for (unsigned int i = 0; i < animation->mNumChannels; i++)
		{
			auto channel = animation->mChannels[i];
			std::string boneName = channel->mNodeName.data;

			if (boneInfoMap.find(boneName) == boneInfoMap.end())
			{
				boneInfoMap[boneName].Id = boneCount;
				boneCount++;
			}
			m_Bones.push_back(std::make_shared<Bone>(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].Id, channel));
		}

		m_BoneDataMap = boneInfoMap;
	}

}