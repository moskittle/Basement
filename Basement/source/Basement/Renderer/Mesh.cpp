#include "bmpch.h"

#include "Mesh.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLDebug.h"
#include "Renderer.h"

namespace Basement {

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<u32> indices, std::vector<SharedPtr<Texture2D>> textures) :
		m_Vertices(vertices),
		m_Indices(indices),
		m_Textures(textures)
	{
		m_VAO = VertexArray::Create();

		m_VBO = VertexBuffer::Create(vertices);
		BufferLayout bufferLayout = {
			{ EShaderDataType::Float3, "a_Position" },
			{ EShaderDataType::Float3, "a_Normal" },
			{ EShaderDataType::Float2, "a_TexCoord" }
		};
		m_VBO->SetLayout(bufferLayout);
		m_VAO->AddVertexBuffer(m_VBO);

		m_IBO = IndexBuffer::Create(indices);
		m_VAO->SetIndexBuffer(m_IBO);
	}

	void Mesh::Draw(SharedPtr<Shader> shader, const glm::mat4& model)
	{
		u32 diffuseIndex = 1;
		u32 specularIndex = 1;
		u32 normalIndex = 1;
		u32 heightIndex = 1;

		for (unsigned int i = 0; i < m_Textures.size(); ++i)
		{
			std::string index;
			std::string& typeName = m_Textures[i]->GetTypeName();
			if (typeName == "texture_diffuse")
			{
				index = std::to_string(diffuseIndex++);
			}
			else if (typeName == "texture_specular")
			{
				index = std::to_string(specularIndex++);
			}
			else if (typeName == "texture_normal")
			{
				index = std::to_string(normalIndex++);
			}
			else if (typeName == "texture_height")
			{
				index = std::to_string(heightIndex++);
			}
			else 
			{
				BM_CORE_ASSERT(true, "Unknown Texture TypeName!");
			}

			shader->Bind();
			std::dynamic_pointer_cast<Basement::OpenGLShader>(shader)->UploadUniform1i(("material." + typeName + index).c_str(), i);

			m_Textures[i]->Activate(i);
		}

		Renderer::Submit(shader, m_VAO, model);
	}

}

