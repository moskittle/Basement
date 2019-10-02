#include "bmpch.h"
#include "Mesh.h"

namespace Basement {

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Shared<Texture2D>> textures)
		: m_Vertices(vertices), m_Indices(indices), m_Textures(textures)
	{
		m_VAO = VertexArray::Create();

		//m_VBO = VertexBuffer::Create(sizeof(float) * m_Vertices.size(), &vertices[0]);


	}

	void Mesh::Draw(Shared<Shader> shader)
	{

	}


}

