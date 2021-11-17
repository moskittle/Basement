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

	void Renderer::ResizeWindow(u32 width, u32 height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
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

	void Renderer::SetClearColor(const glm::vec4& color)
	{
		RenderCommand::SetClearColor(color);
	}

	void Renderer::ClearBufferBit(u32 mask)
	{
		RenderCommand::Clear(mask);
	}

	void Renderer::EnableDepthTest()
	{
		RenderCommand::EnableDepthTest();
	}

	void Renderer::DisableDepthTest()
	{
		RenderCommand::DisableDepthTest();
	}

	void Renderer::EnableStencilTest()
	{
		RenderCommand::EnableStencilTest();
	}

	void Renderer::SetStencilPredicate(u32 predicate, i32 ref, u32 mask)
	{
		RenderCommand::SetStencilFunc(predicate, ref, mask);
	}

	void Renderer::EnableStencilMaskOverwrite()
	{
		Basement::RenderCommand::SetStencilMask(0x00);
	}

	void Renderer::DisableStencilMaskOverwrite()
	{
		RenderCommand::SetStencilMask(0xFF);
	}

	void Renderer::Submit(SharedPtr<Shader> shader, SharedPtr<VertexArray> vertexArray, const glm::mat4& modelMatrix)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Model", modelMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_View", m_SceneData->ViewMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Projection", m_SceneData->ProjectionMatrix);

		vertexArray->Bind();
		RenderCommand::DrawIndex(vertexArray);
	}

	void Renderer::SubmitForPoints(SharedPtr<Shader> shader, SharedPtr<VertexArray> vertexArray, const glm::mat4& modelMatrix)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Model", modelMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_View", m_SceneData->ViewMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Projection", m_SceneData->ProjectionMatrix);

		vertexArray->Bind();
		RenderCommand::DrawIndexWithPoints(vertexArray);
	}

	void Renderer::SubmitForLines(SharedPtr<Shader> shader, SharedPtr<VertexArray> vertexArray, const glm::mat4& modelMatrix)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Model", modelMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_View", m_SceneData->ViewMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Projection", m_SceneData->ProjectionMatrix);

		vertexArray->Bind();
		RenderCommand::DrawIndexWithLines(vertexArray);
	}

	void Renderer::SubmitArrays(SharedPtr<Shader> shader, SharedPtr<VertexArray> vertexArray, u32 first, u32 count, const glm::mat4& modelMatrix)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Model", modelMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_View", m_SceneData->ViewMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Projection", m_SceneData->ProjectionMatrix);

		vertexArray->Bind();
		RenderCommand::DrawArrays(first, count);
	}

	void Renderer::SubmitArraysForPoints(SharedPtr<Shader> shader, SharedPtr<VertexArray> vertexArray, u32 first, u32 count, const glm::mat4& modelMatrix)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Model", modelMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_View", m_SceneData->ViewMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Projection", m_SceneData->ProjectionMatrix);

		vertexArray->Bind();
		RenderCommand::DrawArraysWithPoints(first, count);
	}

	void Renderer::SubmitArraysForLines(SharedPtr<Shader> shader, SharedPtr<VertexArray> vertexArray, u32 first, u32 count, const glm::mat4& modelMatrix)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Model", modelMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_View", m_SceneData->ViewMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Projection", m_SceneData->ProjectionMatrix);

		vertexArray->Bind();
		RenderCommand::DrawArraysWithLines(first, count);
	}

	void Renderer::SubmitArraysForScreen(SharedPtr<Shader> shader, SharedPtr<VertexArray> vertexArray, u32 first, u32 count)
	{
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	void Renderer::SubmitArraysForSkybox(SharedPtr<Shader> shader, SharedPtr<VertexArray> vertexArray, u32 first, u32 count, const glm::mat4& modelMatrix)
	{
		RenderCommand::SetDepthFunc(RendererGL::LEqual);
		SubmitArrays(shader, vertexArray, first, count, modelMatrix);
		RenderCommand::SetDepthFunc(RendererGL::Less);
	}

	void Renderer::SubmitModel(SharedPtr<Model> model, SharedPtr<Shader> shader, const glm::mat4& modelMatrix)
	{
		model->Draw(shader, modelMatrix);
	}


}