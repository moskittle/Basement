#include "bmpch.h"

#include "Animator.h"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/string_cast.hpp>

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
		m_GlobalBoneMatrices.reserve(100);
		for (int i = 0; i < 100; i++)
		{
			m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
			m_GlobalBoneMatrices.push_back(glm::mat4(1.0f));
		}

		CalculateBoneTransform(m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
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

	void Animator::CalculateBoneTransform(const SharedPtr<BoneNode> node, glm::mat4 parentTransform)
	{
		const std::string& nodeName = node->name;
		glm::mat4 localTransformation = node->localTransformation;
		SharedPtr<Bone> bone = m_CurrentAnimation->FindBone(nodeName);

		if (bone)
		{
			bone->Update(m_CurrentTime);
			localTransformation = bone->GetLocalTransformation();
		}

		glm::mat4 globalTransformation = parentTransform * localTransformation;
		auto boneDataMap = m_CurrentAnimation->GetBoneDataMap();
		if (boneDataMap.find(nodeName) != boneDataMap.end())
		{
			int index = boneDataMap[nodeName].Id;
			const glm::mat4& offsetTransformation = boneDataMap[nodeName].offsetMatrix;
			m_GlobalBoneMatrices[index] = globalTransformation;
			m_FinalBoneMatrices[index] = globalTransformation * offsetTransformation;

			// Note: there is a inverse operation
			glm::vec3 jointPosition;
			glm::decompose(glm::inverse(m_FinalBoneMatrices[index]), glm::vec3(), glm::quat(), jointPosition, glm::vec3(), glm::vec4());
			boneDataMap[nodeName].JointVertex.Position = jointPosition;
		}

		auto childSize = static_cast<int>(node->children.size());
		for (int i = 0; i < childSize; ++i)
		{
			CalculateBoneTransform(node->children[i], globalTransformation);
		}

	}

	void Animator::CalculateBoneTransformWithInverseKinematics(const SharedPtr<BoneNode> node, glm::mat4 parentTransform)
	{
		const std::string& nodeName = node->name;
		glm::mat4 localTransformation = node->localTransformation;
		SharedPtr<Bone> bone = m_CurrentAnimation->FindBone(nodeName);

		if (bone)
		{
			if (m_IkBoneNames.count(nodeName) == 0)
			{
				bone->Update(m_CurrentTime);
			}

			localTransformation = bone->GetLocalTransformation();
		}

		glm::mat4 globalTransformation = parentTransform * localTransformation;
		auto boneDataMap = m_CurrentAnimation->GetBoneDataMap();
		if (boneDataMap.find(nodeName) != boneDataMap.end())
		{
			int index = boneDataMap[nodeName].Id;
			const glm::mat4& offsetTransformation = boneDataMap[nodeName].offsetMatrix;
			m_GlobalBoneMatrices[index] = globalTransformation;
			m_FinalBoneMatrices[index] = globalTransformation * offsetTransformation;

			// Note: there is a inverse operation
			glm::vec3 jointPosition;
			glm::decompose(glm::inverse(m_FinalBoneMatrices[index]), glm::vec3(), glm::quat(), jointPosition, glm::vec3(), glm::vec4());
			boneDataMap[nodeName].JointVertex.Position = jointPosition;
		}

		auto childSize = static_cast<int>(node->children.size());
		for (int i = 0; i < childSize; ++i)
		{
			CalculateBoneTransformWithInverseKinematics(node->children[i], globalTransformation);
		}
	}

	void Animator::DrawSkeleton(SharedPtr<Shader> shader, glm::mat4 modelMatrix, bool drawJoints, bool drawBones)
	{
		if (m_CurrentAnimation)
		{
			m_CurrentAnimation->DrawSkeleton(shader, modelMatrix, drawJoints, drawBones);
		}
	}

	void Animator::GenerateInverseKinematicsData(std::string endEffectorName)
	{
		m_EndEffectors = m_CurrentAnimation->GenerateInverseKinematicsData(endEffectorName);
		m_PriorityEndEffectors = m_CurrentAnimation->GenerateInverseKinematicsPriorityData(endEffectorName);

		for (const auto& endEffector : m_EndEffectors)
		{
			m_IkBoneNames.insert(endEffector->name);
		}

		for (const auto& endEffector : m_PriorityEndEffectors)
		{
			m_IkBoneNames.insert(endEffector->name);
		}
	}

	void Animator::SolveInverseKinematicsCCD(const glm::vec3& targetPosition)
	{
		UpdateInverseKinematicsMatrices(targetPosition, m_EndEffectors);
		UpdateInverseKinematicsMatrices(targetPosition, m_PriorityEndEffectors);

		CalculateBoneTransformWithInverseKinematics(m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
	}

	void Animator::UpdateInverseKinematicsMatrices(const glm::vec3& targetPosition, std::vector<SharedPtr<BoneNode>> endEffectors)
	{
		auto boneDataMap = m_CurrentAnimation->GetBoneDataMap();

		const auto& endEffectorCount = endEffectors.size();
		for (int i = 0; i < endEffectorCount - 1; ++i)
		{
			glm::mat4 rotationMatrix = EvalRotationMatrix(i, targetPosition, endEffectors);

			for (int j = 0; j < i; ++j)
			{
				SharedPtr<Bone> bone = m_CurrentAnimation->FindBone(endEffectors[j]->name);
				rotationMatrix *= glm::inverse(bone->GetLocalTransformation());
			}

			m_CurrentAnimation->FindBone(endEffectors[i]->name)->SetLocalTransformation(rotationMatrix);

			for (int k = i; k >= 0; --k)
			{
				int currIndex = boneDataMap[endEffectors[k]->name].Id;
				int parentIndex = boneDataMap[endEffectors[k + 1]->name].Id;

				m_GlobalBoneMatrices[currIndex] = m_GlobalBoneMatrices[parentIndex] * m_CurrentAnimation->FindBone(endEffectors[k]->name)->GetLocalTransformation();
				m_FinalBoneMatrices[currIndex] = m_GlobalBoneMatrices[currIndex] * boneDataMap[endEffectors[k]->name].offsetMatrix;
			}
		}
	}

	glm::mat4 Animator::EvalRotationMatrix(int index, const glm::vec3& targetPosition, std::vector<SharedPtr<BoneNode>> endEffectors)
	{
		auto boneDataMap = m_CurrentAnimation->GetBoneDataMap();

		glm::mat4 toLeafTransform(1.0f);
		for (int i = index; i >= 0; --i)
		{
			SharedPtr<Bone> bone = m_CurrentAnimation->FindBone(endEffectors[i]->name);
			toLeafTransform = toLeafTransform * bone->GetLocalTransformation();
		}

		glm::vec3 v_ck;
		glm::decompose(toLeafTransform, glm::vec3(), glm::quat(), v_ck, glm::vec3(), glm::vec4());
		glm::mat4 parentTransform = m_GlobalBoneMatrices[boneDataMap[endEffectors[index + 1]->name].Id];
		glm::vec3 v_dk = glm::inverse(parentTransform) * glm::vec4(targetPosition, 1.0f);
		v_ck = glm::normalize(v_ck);
		v_dk = glm::normalize(v_dk);
		float cosine = glm::dot(v_ck, v_dk);
		float angle = acos(glm::clamp(cosine, -1.0f, 1.0f)) * 0.2f; // constraint
		glm::vec3 axis = glm::normalize(glm::cross(v_ck, v_dk));
		glm::mat4 rotationMatrix = glm::mat4_cast(glm::angleAxis(angle, axis));

		return rotationMatrix * toLeafTransform;
	}
}