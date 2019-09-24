#include "bmpch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Basement {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::BeginScene(Camera2D& camera)
	{
		m_SceneData->ViewMatrix = camera.GetViewMatrix();
		m_SceneData->ProjectionMatrix = camera.GetProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Shared<Shader>& shader, const Shared<VertexArray>& vertexArray, const glm::mat4& model)
	{
		shader->Bind();

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Model", model);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_View", m_SceneData->ViewMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Projection", m_SceneData->ProjectionMatrix);

		vertexArray->Bind();
		RenderCommand::DrawIndex(vertexArray);
	}

	void Renderer::SubmitArrays(const Shared<Shader>& shader, const Shared<VertexArray>& vertexArray, uint32_t first, uint32_t count, const glm::mat4& model)
	{
		shader->Bind();

		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.f);
		glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Model", model);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_View", View);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Projection", Projection);

		vertexArray->Bind();
		RenderCommand::DrawArrays(vertexArray, first, count);
	}

}