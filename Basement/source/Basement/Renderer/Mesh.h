#pragma once

#include "Texture.h"
#include "Shader.h" 
#include "VertexArray.h"
#include "Buffer.h"

#include <vector>
#include <glm/glm.hpp>

namespace Basement {

	class Mesh
	{
	public:
		Mesh() = default;
		Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Shared<Texture2D>> textures);
		~Mesh() = default;
		
		void Draw(Shared<Shader> shader, const glm::mat4& model);
	private:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		std::vector<Shared<Texture2D>> m_Textures;

		Shared<VertexArray> m_VAO;
		Shared<VertexBuffer> m_VBO;
		Shared<IndexBuffer> m_IBO;
	};




}