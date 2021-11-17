#include "bmpch.h"

#include "Animator.h"

namespace Basement
{
	Animator::Animator(const std::unordered_map<std::string, SharedPtr<Animation>>& animations)
	{
		m_CurrentTime = 0.0;
		m_AnimationLibrary = animations;

		if (!m_AnimationLibrary.empty())
		{
			m_CurrentAnimation = m_AnimationLibrary.begin()->second;
		}

		m_FinalBoneVqses.reserve(100);

		for (int i = 0; i < 100; i++)
		{
			m_FinalBoneVqses.push_back(Vqs());
		}
	}

	void Animator::PlayAnimation(const std::string& animationName)
	{
		if (m_AnimationLibrary.count(animationName) == 0)
		{
			BM_CORE_INFO("{0} is not found in this animator.");
			return;
		}

		m_CurrentAnimation = m_AnimationLibrary[animationName];
		m_CurrentTime = 0.0f;
	}

	void Animator::UpdateAnimation(float dt)
	{
		if (m_CurrentAnimation)
		{
			m_CurrentTime += static_cast<float>(m_CurrentAnimation->GetTicksPerSecond()) * dt;
			m_CurrentTime = static_cast<float>(fmod(m_CurrentTime, m_CurrentAnimation->GetDuration()));
			CalculateBoneTransform(m_CurrentAnimation->GetRootNode(), Vqs());
		}
	}

	void Animator::CalculateBoneTransform(const SharedPtr<BoneNode> node, Vqs parentTransformation)
	{
		const std::string& nodeName = node->name;
		Vqs localTransformation = node->localTransformation;
		SharedPtr<Bone> bone = m_CurrentAnimation->FindBone(nodeName);

		if (bone)
		{
			bone->Update(m_CurrentTime);
			localTransformation = bone->GetLocalTransformation();
		}
		Vqs globalTransformation = parentTransformation * localTransformation;

		auto boneDataMap = m_CurrentAnimation->GetBoneDataMap();
		if (boneDataMap.find(nodeName) != boneDataMap.end())
		{
			int index = boneDataMap[nodeName].Id;
			const Vqs& offsetTransformation = boneDataMap[nodeName].OffsetVqs;
			m_FinalBoneVqses[index] = globalTransformation * offsetTransformation;
			boneDataMap[nodeName].JointVertex.Position = m_FinalBoneVqses[index].v;
		}

		for (int i = 0; i < static_cast<int>(node->children.size()); ++i)
		{
			CalculateBoneTransform(node->children[i], globalTransformation);
		}

	}

	void Animator::DrawSkeleton(SharedPtr<Shader> shader, glm::mat4 modelMatrix, bool drawJoints, bool drawBones)
	{
		if (m_CurrentAnimation)
		{
			m_CurrentAnimation->DrawSkeleton(shader, modelMatrix, drawJoints, drawBones);
		}
	}
}