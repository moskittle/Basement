#include "bmpch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

namespace Basement {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::BeginScene(Camera3D& camera)
	{
		m_SceneData->ViewMatrix = camera.GetViewMatrix();
		m_SceneData->ProjectionMatrix = camera.GetProjectionMatrix();
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

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Model", model);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_View", m_SceneData->ViewMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Projection", m_SceneData->ProjectionMatrix);

		vertexArray->Bind();
		RenderCommand::DrawArrays(vertexArray, first, count);
	}

}