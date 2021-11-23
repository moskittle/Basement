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
		void CalculateBoneTransform(const SharedPtr<BoneNode> node, glm::mat4 parentVqs);
		void DrawSkeleton(SharedPtr<Shader> shader, glm::mat4 modelMatrix, bool drawJoints, bool drawBones);

		const std::unordered_map<std::string, SharedPtr<Animation>>& GetAnimationLibrary() const { return m_AnimationLibrary; }
		SharedPtr<Animation> GetCurrentAnimation() const { return m_CurrentAnimation; }
		const std::vector<glm::mat4>& GetFinalBoneMatrices() { return m_FinalBoneMatrices; }
	private:
		std::vector<glm::mat4> m_FinalBoneMatrices;
		SharedPtr<Animation> m_CurrentAnimation;
		std::unordered_map<std::string, SharedPtr<Animation>> m_AnimationLibrary;
		float m_CurrentTime;
	};

}