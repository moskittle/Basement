#include "bmpch.h"

#include "Animator.h"

#include <glm/gtx/matrix_decompose.hpp>

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

		m_FinalBoneMatrices.reserve(100);

		for (int i = 0; i < 100; i++)
		{
			m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
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
			CalculateBoneTransform(m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
		}
	}

	void Animator::CalculateBoneTransform(const SharedPtr<BoneNode> node, glm::mat4 parentTransformation)
	{
		const std::string& nodeName = node->name;
		glm::mat4 localTransformation = node->localTransformation;
		SharedPtr<Bone> bone = m_CurrentAnimation->FindBone(nodeName);

		if (bone)
		{
			bone->Update(m_CurrentTime);
			localTransformation = bone->GetLocalTransformation();
		}
		glm::mat4 globalTransformation = parentTransformation * localTransformation;

		auto boneDataMap = m_CurrentAnimation->GetBoneDataMap();
		if (boneDataMap.find(nodeName) != boneDataMap.end())
		{
			int index = boneDataMap[nodeName].Id;
			const glm::mat4& offsetTransformation = boneDataMap[nodeName].offsetMatrix;
			m_FinalBoneMatrices[index] = globalTransformation * offsetTransformation;
			glm::vec3 jointPosition;

			// Note: there is a inverse operation
			glm::decompose(glm::inverse(m_FinalBoneMatrices[index]), glm::vec3(), glm::quat(), jointPosition, glm::vec3(), glm::vec4());
			boneDataMap[nodeName].JointVertex.Position = jointPosition;
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