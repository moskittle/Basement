#pragma once

#include "Particle.h"
#include "ParticleConstaint.h"

#include <vector>

namespace Basement
{
	class Cloth
	{
	public:
		Cloth(float width, float height, int parcleCountInWidth, int parcleCountInHeight);
		~Cloth() = default;

		void Init();
		void Update(float dt);
		void Draw();
		void AddForce(const glm::vec3& force);
		void WindForce(const glm::vec3& force);
		void CheckSphereCollsion(const glm::vec3& spherePosition, float radius);

	private:
		Particle* GetParticle(int row, int col);
		void MakeConstraints(Particle* p1, Particle* p2);
		glm::vec3 CalcTriangleNormal(Particle* p1, Particle* p2, Particle* p3);
		void AddWindForcesForTriangle(Particle* p1, Particle* p2, Particle* p3, const glm::vec3& direction);
		void DrawTriangle(Particle* p1, Particle* p2, Particle* p3, const glm::vec3& color);

		int m_ParticleCountInWidth;
		int m_ParticleCountInHeight;
		std::vector<Particle> m_Particles;
		std::vector<ParticleConstraint> m_Constraints;

		static int s_ConstraintIterations;

	};
}