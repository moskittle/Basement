#pragma once

#include "Mesh3D.h"

#include <vector>

namespace Basement {

	class Sphere : public Mesh3D
	{
	public:
		Sphere(int mesh_size);
		~Sphere();

		virtual u32			GetVertexCount() override;
		virtual float*				GetVertexArray() override;
		virtual float*				GetNormalArray() override;
		virtual u32			GetFaceCount() override;
		virtual u32*			GetFaceArray() override;
	private:
		std::vector<glm::vec4> m_Vertices;
		std::vector<glm::vec4> m_Normals;
		std::vector<Face> m_Faces;
		float* m_VertexArray;
		float* m_NormalsArray;
		u32* m_IndexArray;
	};

}