#pragma once

#include "Texture.h"
#include "Shader.h" 
#include "VertexArray.h"
#include "Buffer.h"

#include <glm/glm.hpp>

#include <assimp/color4.h>

namespace Basement {

	class MaterialAttrib 
	{
	public:
		MaterialAttrib() = default;

		void SetAttributes(const aiColor4D& ambient, const aiColor4D& diffuse, const aiColor4D& specular, float shininess)
		{
			m_AmbientColor = glm::vec3(ambient.r, ambient.g, ambient.b);
			m_DiffuseColor = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
			m_SpecularColor = glm::vec3(specular.r, specular.g, specular.b);
			m_Shininess = shininess;
		}

		const glm::vec3& GetAmbientColor() const { return m_AmbientColor; }
		const glm::vec3& GetDiffuseColor() const { return m_DiffuseColor; }
		const glm::vec3& GetSpecularColor() const { return m_SpecularColor; }
		float			 GetShininess() const { return m_Shininess; }

	private:
		glm::vec3 m_AmbientColor;
		glm::vec3 m_DiffuseColor;
		glm::vec3 m_SpecularColor;

		float m_Shininess;
	};

	class Mesh
	{
	public:
		Mesh(const std::vector<Vertex>& vertices, const std::vector<u32>& indices, const std::vector<SharedPtr<Texture2D>>& textures, const MaterialAttrib& materialAttrib);
		~Mesh() = default;
		
		void Draw(SharedPtr<Shader> shader, const glm::mat4& model);
	private:
		std::vector<Vertex> m_Vertices;
		std::vector<u32> m_Indices;
		std::vector<SharedPtr<Texture2D>> m_Textures;
		MaterialAttrib m_MaterialAttrib;

		SharedPtr<VertexArray> m_VAO;
		SharedPtr<VertexBuffer> m_VBO;
		SharedPtr<IndexBuffer> m_IBO;
	};

}