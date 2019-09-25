#include "bmpch.h"

#include "Sphere.h"

#include <cmath>

const float PI = 4.0f * std::atan(1.0f);

namespace Basement {



#define M (2 * size)
#define N (size)
#define NORTH (M*(N-1))
#define SOUTH (M*(N-1)+1)
	Sphere::Sphere(int size) : m_Vertices(M * (N - 1) + 2), m_Normals(M * (N - 1) + 2) 
	{
		for (int i = 1; i < N; ++i)
		{
			float theta = PI * i / N;
			for (int j = 0; j < M; ++j)
			{
				int index = M * (i - 1) + j;
				float phi = 2 * PI * j / M;
				m_Normals[index].x = sin(theta) * cos(phi);
				m_Normals[index].y = sin(theta) * sin(phi);
				m_Normals[index].z = cos(theta);
				m_Normals[index].w = 0;
			}
		}
		m_Normals[NORTH] = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
		m_Normals[SOUTH] = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

		for (unsigned n = 0; n < m_Normals.size(); ++n)
		{
			m_Vertices[n] = m_Normals[n];
			m_Vertices[n].w = 1;
		}

		for (int i = 2; i < N; ++i)
		{
			for (int j = 0; j < M; ++j)
			{
				Face face;
				int jp1 = (j + 1) % M;
				face[0] = M * (i - 2) + j;
				face[1] = M * (i - 2) + jp1;
				face[2] = M * (i - 1) + jp1;
				m_Faces.push_back(face);
				face[1] = face[2];
				face[2] = M * (i - 1) + j;
				m_Faces.push_back(face);
			}
		}
		for (int j = 0; j < M; ++j)
		{
			Face face;
			int jp1 = (j + 1) % M;
			face[0] = j;
			face[1] = jp1;
			face[2] = NORTH;
			m_Faces.push_back(face);
			face[0] = M * (N - 2) + j;
			face[1] = M * (N - 2) + jp1;
			face[2] = SOUTH;
			m_Faces.push_back(face);
		}

	}
#undef SOUTH
#undef NORTH
#undef N
#undef M

	Sphere::~Sphere()
	{
		//delete m_VertexArray;
		//delete m_NormalsArray;
		//delete m_IndexArray;
	}

	uint32_t Sphere::GetVertexCount()
	{
		return static_cast<uint32_t>(m_Vertices.size());
	}

	float* Sphere::GetVertexArray()
	{
		int size = 4 * GetVertexCount();
		m_VertexArray = new float[size];
		for(int i = 0; i < size; i += 4)
		{
			int index = i / 4;
			m_VertexArray[i] = m_Vertices[index].x;
			m_VertexArray[i+1] = m_Vertices[index].y;
			m_VertexArray[i+2] = m_Vertices[index].z;
			m_VertexArray[i+3] = 1.0f;
			//BM_CORE_TRACE("Index{0}: {1}, {2}, {3}", i, m_VertexArray[i], m_VertexArray[i + 1], m_VertexArray[i + 2]);
		}
		
		return m_VertexArray;
	}

	float* Sphere::GetNormalArray()
	{
		return nullptr;
	}

	uint32_t Sphere::GetFaceCount()
	{
		return static_cast<uint32_t>(m_Faces.size());
	}

	uint32_t* Sphere::GetFaceArray()
	{
		int size = 3 * GetFaceCount();
		m_IndexArray = new uint32_t[size];
		for (int i = 0; i < size; i += 3)
		{
			int index = i / 3;
			m_IndexArray[i] = m_Faces[index][0];
			m_IndexArray[i+1] = m_Faces[index][1];
			m_IndexArray[i+2] = m_Faces[index][2];
			//BM_CORE_TRACE("Index{0}: {1}, {2}, {3}", i, m_IndexArray[i], m_IndexArray[i + 1], m_IndexArray[i + 2]);
		}

		return m_IndexArray;
	}

}