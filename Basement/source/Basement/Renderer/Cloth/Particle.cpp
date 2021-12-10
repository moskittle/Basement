#include "bmpch.h"
#include "Particle.h"

namespace Basement
{
	float Particle::s_Damping = 0.01f;

	Particle::Particle(const glm::vec3& position) :
		m_Position(position), m_PreviousPosition(position), m_Acceleration(glm::vec3(0.0f)),
		m_Mass(1.0f), m_IsFixed(false), m_AccumulatedNormal(glm::vec3(0.0f))
	{
	}

	/// <summary>
	/// Add force by using Newton's law: F = m * a. And we are doing integration here.
	/// </summary>
	/// <param name="force"></param>
	void Particle::AddForce(glm::vec3 force)
	{
		BM_ASSERT("Mass cannot be negative!", m_Mass > 0.0f);
		m_Acceleration += force / m_Mass;
	}

	/// <summary>
	/// Update the position of each mass point
	/// </summary>
	/// <param name="dt"></param>
	void Particle::Update(float dt)
	{
		if (m_IsFixed == false)
		{
			glm::vec3 temp = m_Position;
			m_Position = m_Position + (m_Position - m_PreviousPosition) * (1.0f - s_Damping) + m_Acceleration * dt;
			m_PreviousPosition = temp;
			// acceleration is reset since it HAS been translated into a change in position (and implicitely into velocity)	
			m_Acceleration = glm::vec3(0.0f);
		}
	}

	/// <summary>
	/// Update the offset position of the particle.
	/// </summary>
	/// <param name="offset"></param>
	void Particle::OffsetPosition(const glm::vec3& offset)
	{
		if (m_IsFixed == false)
		{
			m_Position += offset;
		}
	}

	//void Particle::AddToNormal(const glm::vec3& normal)
	//{
	//	m_AccumulatedNormal += glm::normalize(normal);
	//}

}