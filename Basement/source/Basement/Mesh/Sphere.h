#pragma once

#include "Mesh3D.h"

#include <vector>

namespace Basement {

	class Sphere : public Mesh3D
	{
	public:
		Sphere(int mesh_size);
		~Sphere();

		virtual uint32_t			GetVertexCount() override;
		virtual float*				GetVertexArray() override;
		virtual float*				GetNormalArray() override;
		virtual uint32_t			GetFaceCount() override;
		virtual uint32_t*			GetFaceArray() override;
	private:
		std::vector<glm::vec4> m_Vertices;
		std::vector<glm::vec4> m_Normals;
		std::vector<Face> m_Faces;
		float* m_VertexArray;
		float* m_NormalsArray;
		uint32_t* m_IndexArray;
	};

}