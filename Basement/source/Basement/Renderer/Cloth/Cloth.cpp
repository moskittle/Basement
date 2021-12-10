#include "bmpch.h"

#include "Cloth.h"
#include "../Renderer.h"

#include <glad/glad.h>

namespace Basement
{
	int Cloth::s_ConstraintIterations = 15;


	Cloth::Cloth(float width, float height, int particleCountInWidth, int particleCountInHeight) :
		m_ParticleCountInWidth(particleCountInWidth), m_ParticleCountInHeight(particleCountInHeight)
	{
		int totalCount = m_ParticleCountInWidth * m_ParticleCountInHeight;
		m_Particles.resize(totalCount);

		// creating particles in a grid of particles from (0,0,0) to (width,-height,0)
		for (int x = 0; x < m_ParticleCountInWidth; ++x)
		{
			for (int y = 0; y < m_ParticleCountInHeight; ++y)
			{
				glm::vec3 position = glm::vec3(width * (x / (float)m_ParticleCountInWidth), -height * (y / (float)m_ParticleCountInHeight), 0);
				m_Particles[y * m_ParticleCountInWidth + x] = Particle(position); // insert particle in column x at y'th row
			}
		}

		for (int x = 0; x < m_ParticleCountInWidth; ++x)
		{
			for (int y = 0; y < m_ParticleCountInHeight; ++y)
			{
				// Connecting immediate neighbor particles with constraints (distance 1 and sqrt(2) in the grid)
				if (x < m_ParticleCountInWidth - 1) { MakeConstraints(GetParticle(x, y), GetParticle(x + 1, y)); }
				if (y < m_ParticleCountInHeight - 1) { MakeConstraints(GetParticle(x, y), GetParticle(x, y + 1)); }
				if (x < m_ParticleCountInWidth - 1 && y < m_ParticleCountInHeight - 1) { MakeConstraints(GetParticle(x, y), GetParticle(x + 1, y + 1)); }
				if (x < m_ParticleCountInWidth - 1 && y < m_ParticleCountInHeight - 1) { MakeConstraints(GetParticle(x + 1, y), GetParticle(x, y + 1)); }

				// Connecting secondary neighbors with constraints (distance 2 and sqrt(4) in the grid)
				if (x < m_ParticleCountInWidth - 2) MakeConstraints(GetParticle(x, y), GetParticle(x + 2, y));
				if (y < m_ParticleCountInHeight - 2) MakeConstraints(GetParticle(x, y), GetParticle(x, y + 2));
				if (x < m_ParticleCountInWidth - 2 && y < m_ParticleCountInHeight - 2) MakeConstraints(GetParticle(x, y), GetParticle(x + 2, y + 2));
				if (x < m_ParticleCountInWidth - 2 && y < m_ParticleCountInHeight - 2) MakeConstraints(GetParticle(x + 2, y), GetParticle(x, y + 2));
			}
		}

		// making the upper left most three and right most three particles unmovable
		for (int i = 0; i < 1; ++i)
		{
			Particle* leftParticle = GetParticle(0 + i, 0);
			leftParticle->OffsetPosition(glm::vec3(0.5f, 0.0f, 0.0f));	// moving the particle a bit towards the center, to make it hang more natural
			leftParticle->SetIsFixed(true);

			Particle* rightParticle = GetParticle(m_ParticleCountInWidth - 1 - i, 0);
			rightParticle->OffsetPosition(glm::vec3(-0.5f, 0.0f, 0.0f));	// moving the particle a bit towards the center, to make it hang more natural 
			rightParticle->SetIsFixed(true);
		}

		for (const auto& particle : m_Particles)
		{
			m_ParticleOriginalPositions.push_back(particle.GetPosition());
		}

		SetupVertexArrayData();
	}

	/// <summary>
	/// Update all the mass points in the cloth with constraints. 
	/// </summary>
	/// <param name="dt"></param>
	void Cloth::Update(float dt)
	{
		for (int i = 0; i < s_ConstraintIterations; i++) // iterate over all constraints several times
		{
			for (auto constraint = m_Constraints.begin(); constraint != m_Constraints.end(); constraint++)
			{
				constraint->SatisfyConstraint(); // satisfy constraint.
			}
		}

		for (auto particle = m_Particles.begin(); particle != m_Particles.end(); particle++)
		{
			particle->Update(dt); // calculate the position of each particle at the next time step.
		}
	}

	/// <summary>
	/// Renders cloth as vertex for now. Will be impoved to render triangles later.
	/// </summary>
	/// <param name="shader"></param>
	/// <param name="modelMatrix"></param>
	void Cloth::DrawParticle(SharedPtr<Shader> shader, glm::mat4 modelMatrix)
	{
		u32 indexCount = static_cast<u32>(m_ClothIndices.size());
		Renderer::SubmitArraysForPoints(shader, m_ClothVao, m_ClothIndices[0], indexCount, modelMatrix);
	}

	void Cloth::AddForce(const glm::vec3& force)
	{
		// Note: prevent delta time too long causing crazy force
		if (glm::length(force) > 5.0f) { return; }

		for (auto particle = m_Particles.begin(); particle != m_Particles.end(); particle++)
		{
			particle->AddForce(force); // add the forces to each particle
		}
	}

	/// <summary>
	/// Add a force from wind during simulation to make the cloth hanging in the air. 
	/// </summary>
	/// <param name="force"></param>
	void Cloth::WindForce(const glm::vec3& force)
	{
		for (int x = 0; x < m_ParticleCountInWidth - 1; x++)
		{
			for (int y = 0; y < m_ParticleCountInHeight - 1; y++)
			{
				AddWindForcesForTriangle(GetParticle(x + 1, y), GetParticle(x, y), GetParticle(x, y + 1), force);
				AddWindForcesForTriangle(GetParticle(x + 1, y + 1), GetParticle(x + 1, y), GetParticle(x, y + 1), force);
			}
		}
	}

	/// <summary>
	/// This method does the fast and simple sphere collision detection
	/// </summary>
	/// <param name="spherePosition"></param>
	/// <param name="radius"></param>
	void Cloth::CheckSphereCollsion(const glm::vec3& spherePosition, float radius)
	{
		for (auto particle = m_Particles.begin(); particle != m_Particles.end(); particle++)
		{
			glm::vec3 dir = particle->GetPosition() - spherePosition;
			float distance = glm::length(dir);
			if (distance < radius) // if the particle is inside the ball
			{
				particle->OffsetPosition(glm::normalize(dir) * (radius - distance));
			}
		}
	}

	glm::vec3 Cloth::GetParticleOffsetPosition(int index)
	{
		BM_ASSERT("index cannot be negative!", index >= 0);
		return m_Particles[index].GetPosition() - m_ParticleOriginalPositions[index];
	}

	/// <summary>
	/// Set up the vertex array and vertex buffer for rendering the cloth.
	/// </summary>
	void Cloth::SetupVertexArrayData()
	{
		// Setup cloth particle
		int count = 0;
		for (auto particle = m_Particles.begin(); particle != m_Particles.end(); particle++)
		{
			auto newVertex = Vertex();
			newVertex.Position = particle->GetPosition();
			newVertex.BoneIds[0] = count;
			m_ClothVertices.push_back(newVertex);
			count++;
		}

		for (int i = 0; i < m_ClothVertices.size(); ++i)
		{
			m_ClothIndices.push_back(i);
		}

		BufferLayout bufferLayout = {
			{ EShaderDataType::Float3, "a_Position" },
			{ EShaderDataType::Float3, "a_Normal" },
			{ EShaderDataType::Float2, "a_TexCoord" },
			{ EShaderDataType::Int4, "a_BoneIds"},
			{ EShaderDataType::Float4, "a_BoneWeights"}
		};

		m_ClothVao = VertexArray::Create();
		m_ClothVbo = VertexBuffer::Create(m_ClothVertices);
		m_ClothVbo->SetLayout(bufferLayout);
		m_ClothVao->AddVertexBuffer(m_ClothVbo);


		//for (u32 i = 0; i < static_cast<u32>(m_PointVertices.size()); ++i)
		//{
		//	m_PointIndices.push_back(i);
		//}

		//m_PathPointIbo = IndexBuffer::Create(m_PointIndices);
		//m_PathPointVao->SetIndexBuffer(m_PathPointIbo);
	}

	Particle* Cloth::GetParticle(int x, int y)
	{
		return &m_Particles[y * m_ParticleCountInWidth + x];
	}

	void Cloth::MakeConstraints(Particle* p1, Particle* p2)
	{
		m_Constraints.push_back(ParticleConstraint(p1, p2));
	}

	glm::vec3 Cloth::CalcTriangleNormal(Particle* p1, Particle* p2, Particle* p3)
	{
		glm::vec3 pos1 = p1->GetPosition();
		glm::vec3 pos2 = p2->GetPosition();
		glm::vec3 pos3 = p3->GetPosition();

		glm::vec3 v1 = pos2 - pos1;
		glm::vec3 v2 = pos3 - pos1;

		return glm::cross(v1, v2);
	}

	void Cloth::AddWindForcesForTriangle(Particle* p1, Particle* p2, Particle* p3, const glm::vec3& direction)
	{
		glm::vec3 normal = CalcTriangleNormal(p1, p2, p3);
		glm::vec3 d = glm::normalize(normal);
		glm::vec3 force = normal * glm::dot(d, direction);
		p1->AddForce(force);
		p2->AddForce(force);
		p3->AddForce(force);
	}

}
