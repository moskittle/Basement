#pragma once
#pragma once

#include "Animation.h"

namespace Basement
{

	class Animator
	{
	public:
		Animator() = default;
		Animator(const std::unordered_map<std::string, SharedPtr<Animation>>& animations);
		~Animator() = default;

		void PlayAnimation(const std::string& animationName);
		void UpdateAnimation(float dt);
		void DrawSkeleton(SharedPtr<Shader> shader, glm::mat4 modelMatrix, bool drawJoints, bool drawBones);
		void GenerateInverseKinematicsData(std::string endEffectorName);
		void SolveInverseKinematicsCCD(const glm::vec3& targetPosition);

		const std::unordered_map<std::string, SharedPtr<Animation>>& GetAnimationLibrary() const { return m_AnimationLibrary; }
		SharedPtr<Animation> GetCurrentAnimation() const { return m_CurrentAnimation; }
		const std::vector<glm::mat4>& GetFinalBoneMatrices() { return m_FinalBoneMatrices; }
		const std::vector<glm::mat4>& GetGlobalBoneMatrices() { return m_GlobalBoneMatrices; }
	private:
		void UpdateInverseKinematicsMatrices(const glm::vec3& targetPosition, std::vector<SharedPtr<BoneNode>> endEffectors);
		void CalculateBoneTransform(const SharedPtr<BoneNode> node, glm::mat4 parentTransform);
		void CalculateBoneTransformWithInverseKinematics(const SharedPtr<BoneNode> node, glm::mat4 parentTransform);
		glm::mat4 EvalRotationMatrix(int index, const glm::vec3& targetPosition, std::vector<SharedPtr<BoneNode>> endEffectors);

		std::vector<SharedPtr<BoneNode>> m_EndEffectors;
		std::vector<SharedPtr<BoneNode>> m_PriorityEndEffectors;
		std::unordered_set<std::string> m_IkBoneNames;
		std::vector<glm::mat4> m_GlobalBoneMatrices;
		std::vector<glm::mat4> m_FinalBoneMatrices;
		SharedPtr<Animation> m_CurrentAnimation;
		std::unordered_map<std::string, SharedPtr<Animation>> m_AnimationLibrary;
		float m_CurrentTime;
	};

}