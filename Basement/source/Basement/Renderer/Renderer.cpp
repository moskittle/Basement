#include "bmpch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Basement {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();

	void Renderer::BeginScene(Camera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<ShaderProgram>& shaderProgram, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& modelMatrix)
	{
		shaderProgram->Bind();
		std::dynamic_pointer_cast<OpenGLShaderProgram>(shaderProgram)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShaderProgram>(shaderProgram)->UploadUniformMat4("u_ModelMatrix", modelMatrix);
		
		vertexArray->Bind();
		RenderCommand::DrawIndex(vertexArray);
	}

}