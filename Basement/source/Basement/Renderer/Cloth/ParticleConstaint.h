#pragma once

#include "Particle.h"
#include "Basement/Core/Core.h"

namespace Basement
{
	class ParticleConstraint
	{
	public:
		ParticleConstraint(Particle* p1, Particle* p2)
		{
			m_P1 = p1;
			m_P2 = p2;

			m_RestDistance = glm::length(p1->GetPosition() - p2->GetPosition());
		}

		/// <summary>
		/// If the distance between two constrained mass point is not the same as rest distance, 
		/// the following calculation will pull or push them to persist the rest distance. 
		/// </summary>
		void SatisfyConstraint()
		{
			glm::vec3 p1ToP2 = m_P2->GetPosition() - m_P1->GetPosition();
			float currentDistance = glm::length(p1ToP2);
			// The offset vector that could moves p1 into a distance of restDistance to p2
			glm::vec3 correctionVector = p1ToP2 * (1.0f - m_RestDistance / currentDistance);
			correctionVector *= 0.5f; // half the correction vector to move both p1 and p2
			m_P1->OffsetPosition(correctionVector);
			m_P2->OffsetPosition(-correctionVector);
		}

	private:
		Particle* m_P1;
		Particle* m_P2;
		float m_RestDistance; // the length between particle p1 and p2 in rest configuration
	};
}