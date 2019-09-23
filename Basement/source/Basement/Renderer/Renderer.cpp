#include "bmpch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Basement {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Shared<Shader>& shader, const Shared<VertexArray>& vertexArray, const glm::mat4& modelMatrix)
	{
		shader->Bind();

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ModelMatrix", modelMatrix);
		
		vertexArray->Bind();
		RenderCommand::DrawIndex(vertexArray);
	}

}