#include "bmpch.h"

#include "Cloth.h"

#include "glad/glad.h"

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

	}

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
	/// drawing the cloth as a smooth shaded (and colored according to column) OpenGL triangular mesh
	/// Called from the display() method
	/// 	The cloth is seen as consisting of triangles for four particles in the grid as follows :
	/// 
	/// (x, y)		*--* (x + 1, y)
	/// 			| /|
	/// 			|/ |
	/// (x, y + 1)	*--* (x + 1, y + 1)
	/// </summary>
	void Cloth::Draw()
	{
		// reset normals from to last frame
		for (auto particle = m_Particles.begin(); particle != m_Particles.end(); particle++)
		{
			particle->SetAccumulatedNormal(glm::vec3(0.0f));
		}

		//create smooth per particle normals by adding up all the (hard) triangle normals that each particle is part of
		for (int x = 0; x < m_ParticleCountInWidth - 1; ++x)
		{
			for (int y = 0; y < m_ParticleCountInHeight - 1; ++y)
			{
				Particle* p1 = GetParticle(x + 1, y);
				Particle* p2 = GetParticle(x, y);
				Particle* p3 = GetParticle(x, y + 1);
				Particle* p4 = GetParticle(x + 1, y + 1);

				glm::vec3 normal = CalcTriangleNormal(p1, p2, p3);
				p1->AddToNormal(normal);
				p2->AddToNormal(normal);
				p3->AddToNormal(normal);

				normal = CalcTriangleNormal(p4, p1, p3);
				p4->AddToNormal(normal);
				p1->AddToNormal(normal);
				p3->AddToNormal(normal);
			}
		}

		glBegin(GL_TRIANGLES);
		for (int x = 0; x < m_ParticleCountInWidth - 1; ++x)
		{
			for (int y = 0; y < m_ParticleCountInHeight - 1; ++y)
			{
				glm::vec3 color(0.0f);
				// red and white color is interleaved according to which column number
				if (x % 2)
				{
					color = glm::vec3(0.6f, 0.2f, 0.2f);
				}
				else
				{
					color = glm::vec3(1.0f, 1.0f, 1.0f);
				}

				Particle* p1 = GetParticle(x + 1, y);
				Particle* p2 = GetParticle(x, y);
				Particle* p3 = GetParticle(x, y + 1);
				Particle* p4 = GetParticle(x + 1, y + 1);

				DrawTriangle(p1, p2, p3, color);
				DrawTriangle(p4, p1, p3, color);
			}
		}
		glEnd();
	}

	void Cloth::AddForce(const glm::vec3& force)
	{
		for (auto particle = m_Particles.begin(); particle != m_Particles.end(); particle++)
		{
			particle->AddForce(force); // add the forces to each particle
		}
	}

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

	/// <summary>
	/// A private method used by drawShaded(), that draws a single triangle p1,p2,p3 with a color
	/// </summary>
	/// <param name="p1"></param>
	/// <param name="p2"></param>
	/// <param name="p3"></param>
	/// <param name="color"></param>
	void Cloth::DrawTriangle(Particle* p1, Particle* p2, Particle* p3, const glm::vec3& color)
	{
		//glColor3fv((GLfloat*)&color);

		glNormal3fv((GLfloat*)&(glm::normalize(p1->GetAccumulatedNormal())));
		glVertex3fv((GLfloat*)&(p1->GetPosition()));

		glNormal3fv((GLfloat*)&(glm::normalize(p2->GetAccumulatedNormal())));
		glVertex3fv((GLfloat*)&(p2->GetPosition()));

		glNormal3fv((GLfloat*)&(glm::normalize(p3->GetAccumulatedNormal())));
		glVertex3fv((GLfloat*)&(p3->GetPosition()));
	}

	void Cloth::Init()
	{
		glShadeModel(GL_SMOOTH);
		glClearColor(0.2f, 0.2f, 0.4f, 0.5f);
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_COLOR_MATERIAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	}


}
