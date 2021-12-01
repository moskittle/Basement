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
			m_GlobalBoneMatrices[index] = globalTransformation;
			m_FinalBoneMatrices[index] = globalTransformation * offsetTransformation;
			boneDataMap[nodeName].globalTransformMatrix = globalTransformation;
			boneDataMap[nodeName].localTransformMatrix = localTransformation;
			boneDataMap[nodeName].globalTransformMatrix = globalTransformation;

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
	}

	void Animator::SolveInverseKinematicsCCD(const glm::vec3& targetPosition)
	{
		const auto& endEffectorCount = m_EndEffectors.size();

		glm::mat4 rotationMatrix = glm::mat4(1.0f);
		for (int i = 0; i < endEffectorCount - 1; ++i)
		{
			//glm::vec3 startPosition;
			//glm::decompose(glm::inverse(m_EndEffectors[i]->globalTransformation), glm::vec3(), glm::quat(), startPosition, glm::vec3(), glm::vec4());
			//glm::vec3 endPosition;
			//glm::vec3 endPosition = glm::inverse(m_EndEffectors[i + 1]->globalTransformation) * glm::vec4(targetPosition, 1.0f);

			//glm::vec3 toTarget = glm::normalize(targetPosition - startPosition);
			//glm::vec3 toEnd = glm::normalize(endPosition - startPosition);

			auto boneDataMap = m_CurrentAnimation->GetBoneDataMap();

			glm::mat4 toLeafTransform(1.0f);
			for (int j = i; j >= 0; --j)
			{
				auto bone = m_CurrentAnimation->FindBone(m_EndEffectors[j]->name);
				toLeafTransform *= bone->GetLocalTransformation();
			}

			glm::vec3 startPosition;
			glm::decompose(toLeafTransform, glm::vec3(), glm::quat(), startPosition, glm::vec3(), glm::vec4());
			//glm::vec3 endPosition2 = glm::inverse(m_EndEffectors[i + 1]->globalTransformation) * glm::vec4(targetPosition, 1.0f);
			glm::vec3 endPosition = glm::inverse(m_GlobalBoneMatrices[boneDataMap[m_EndEffectors[i + 1]->name].Id]) * glm::vec4(targetPosition, 1.0f);

			BM_CORE_INFO("startPosition :" + glm::to_string(startPosition));
			BM_CORE_INFO("endPosition :" + glm::to_string(endPosition));

			glm::vec3 toTarget = glm::normalize(startPosition);
			glm::vec3 toEnd = glm::normalize(endPosition);

			float cosine = glm::dot(toTarget, toEnd);
			if (cosine < 1.0f)
			{
				glm::vec3 rotationAxis = glm::cross(toEnd, toTarget);
				float rotationAngle = glm::angle(toEnd, toTarget);
				glm::quat rotation = glm::angleAxis(rotationAngle, rotationAxis);
				rotation = glm::normalize(rotation);

				// rotate
				rotationMatrix = glm::mat4(rotation) * toLeafTransform;
			}

			for (int j = 0; j <= i; ++j)
			{
				auto localMat = m_CurrentAnimation->FindBone(m_EndEffectors[j]->name)->GetLocalTransformation();
				//rotationMatrix *= glm::inverse(m_EndEffectors[j]->localTransformation);
				rotationMatrix *= glm::inverse(localMat);
			}

			m_EndEffectors[i]->ikTransformation = rotationMatrix;

			for (int k = i; k >= 0; --k)
			{
				auto currentEndEffector = m_EndEffectors[k];
				int currentIndex = boneDataMap[currentEndEffector->name].Id;
				auto parentEndEffector = m_EndEffectors[k + 1];
				int parentIndex = boneDataMap[parentEndEffector->name].Id;

				//currentEndEffector->globalTransformation = parentEndEffector->globalTransformation * currentEndEffector->localTransformation;
				//m_FinalBoneMatrices[currentIndex] = currentEndEffector->globalTransformation * boneDataMap[currentEndEffector->name].offsetMatrix;
				auto localMat = m_CurrentAnimation->FindBone(currentEndEffector->name)->GetLocalTransformation();
				m_GlobalBoneMatrices[currentIndex] = m_GlobalBoneMatrices[parentIndex] * localMat;
				m_FinalBoneMatrices[currentIndex] = m_GlobalBoneMatrices[currentIndex] * boneDataMap[currentEndEffector->name].offsetMatrix;
			}
		}
	}
}