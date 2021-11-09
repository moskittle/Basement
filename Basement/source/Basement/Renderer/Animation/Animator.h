#pragma once
#pragma once

#include "Vqs.h"
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
		void CalculateBoneTransform(const SharedPtr<BoneNode> node, Vqs parentVqs);

		const std::unordered_map<std::string, SharedPtr<Animation>>& GetAnimationLibrary() const { return m_AnimationLibrary; }
		SharedPtr<Animation> GetCurrentAnimation() const { return m_CurrentAnimation; }
		const std::vector<Vqs>& GetFinalBoneVqses() { return m_FinalBoneVqses; }
	private:
		std::vector<Vqs> m_FinalBoneVqses;
		SharedPtr<Animation> m_CurrentAnimation;
		std::unordered_map<std::string, SharedPtr<Animation>> m_AnimationLibrary;
		float m_CurrentTime;
	};

}