#pragma once

#include "Particle.h"
#include "Basement/Core/Core.h"

namespace Basement
{
	class ParticleConstraint
	{
	public:
		ParticleConstraint(const Particle& p1, const Particle& p2)
		{
			m_P1 = std::make_shared<Particle>(p1);
			m_P2 = std::make_shared<Particle>(p2);

			m_RestDistance = glm::length(p1.GetPosition() - p2.GetPosition());
		}

		void SatisfyConstraint()
		{
			glm::vec3 p1ToP2 = m_P2->GetPosition() - m_P1->GetPosition();
			float currentDistance = glm::length(p1ToP2);
			// The offset vector that could moves p1 into a distance of rest_distance to p2
			glm::vec3 correctionVector = p1ToP2 * (1.0f - m_RestDistance / currentDistance);
			correctionVector *= 0.5f; // half the correction vector to move both p1 and p2
			m_P1->OffsetPosition(correctionVector);
			m_P2->OffsetPosition(-correctionVector);
		}

	private:
		SharedPtr<Particle> m_P1;
		SharedPtr<Particle> m_P2;
		float m_RestDistance; // the length between particle p1 and p2 in rest configuration
	};
}