#pragma once


#include <glm/glm.hpp>


namespace Basement
{

	class Particle
	{
	public:
		Particle(const glm::vec3& position);
		~Particle() = default;

		void AddForce(glm::vec3 force);
		void Update(float dt);
		void OffsetPosition(const glm::vec3& offset);
		void AddToNormal(const glm::vec3& normal);

		const bool& GetIsFixed() const { return m_IsFixed; }
		void SetIsFixed(bool isFixed) { m_IsFixed = isFixed; }
		const float& GetMass() const { return m_Mass; }
		void SetMass(float mass) { m_Mass = mass; }
		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; }
		const glm::vec3& GetPreviousPosition() const { return m_PreviousPosition; }
		void SetPreviousPosition(const glm::vec3& prevPosition) { m_PreviousPosition = prevPosition; }
		const glm::vec3& GetAcceleration() const { return m_Acceleration; }
		void SetAcceleration(const glm::vec3& acceleration) { m_Acceleration = acceleration; }
		const glm::vec3& GetAccumulatedNormal() const { return m_Acceleration; }
		void SetAccumulatedNormal(const glm::vec3& accumulatedNormal) { m_AccumulatedNormal = accumulatedNormal; }

	private:
		bool m_IsFixed;
		float m_Mass;
		glm::vec3 m_Position;
		glm::vec3 m_PreviousPosition;
		glm::vec3 m_Acceleration;
		glm::vec3 m_AccumulatedNormal;	// Note: not unit length

		static float m_Damping;
	};




}