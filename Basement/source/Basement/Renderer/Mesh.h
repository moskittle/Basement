#pragma once

#include "Texture.h"
#include "Shader.h" 
#include "VertexArray.h"
#include "Buffer.h"

#include <glm/glm.hpp>

namespace Basement {

	class Mesh
	{
	public:
		Mesh() = default;
		Mesh(std::vector<Vertex> vertices, std::vector<u32> indices, std::vector<SharedPtr<Texture2D>> textures);
		~Mesh() = default;
		
		void Draw(SharedPtr<Shader> shader, const glm::mat4& model);
	private:
		std::vector<Vertex> m_Vertices;
		std::vector<u32> m_Indices;
		std::vector<SharedPtr<Texture2D>> m_Textures;

		SharedPtr<VertexArray> m_VAO;
		SharedPtr<VertexBuffer> m_VBO;
		SharedPtr<IndexBuffer> m_IBO;
	};




}