#pragma once

#include "Particle.h"
#include "ParticleConstaint.h"
#include "../Buffer.h"
#include "../VertexArray.h"
#include "../Shader.h"

#include <vector>

namespace Basement
{
	class Cloth
	{
	public:
		Cloth(float width, float height, int parcleCountInWidth, int parcleCountInHeight);
		~Cloth() = default;

		void Update(float dt);
		void DrawParticle(SharedPtr<Shader> shader, glm::mat4 modelMatrix);
		void AddForce(const glm::vec3& force);
		void WindForce(const glm::vec3& force);
		void CheckSphereCollsion(const glm::vec3& spherePosition, float radius);
		int GetParticleCount() { return static_cast<int>(m_Particles.size()); }
		glm::vec3 GetParticleOffsetPosition(int index);

	private:
		void SetupVertexArrayData();
		Particle* GetParticle(int row, int col);
		void MakeConstraints(Particle* p1, Particle* p2);
		glm::vec3 CalcTriangleNormal(Particle* p1, Particle* p2, Particle* p3);
		void AddWindForcesForTriangle(Particle* p1, Particle* p2, Particle* p3, const glm::vec3& direction);

		int m_ParticleCountInWidth;
		int m_ParticleCountInHeight;
		std::vector<Particle> m_Particles;
		std::vector<glm::vec3> m_ParticleOriginalPositions;
		std::vector<ParticleConstraint> m_Constraints;

		// Data for drawing cloth points
		std::vector<Vertex> m_ClothVertices;
		std::vector<u32> m_ClothIndices;
		SharedPtr<VertexArray> m_ClothVao;
		SharedPtr<VertexBuffer> m_ClothVbo;
		SharedPtr<IndexBuffer> m_ClothIbo;

		static int s_ConstraintIterations;
	};
}