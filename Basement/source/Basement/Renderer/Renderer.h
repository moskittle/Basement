#pragma once

#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"
#include "Model.h"

namespace Basement {
	
	class Renderer 
	{
	public:
		static void Init();
		static void ResizeWindow(u32 width, u32 height);

		// Scene
		static void BeginScene(Camera3D& camera);
		static void BeginScene(Camera2D& camera);
		static void EndScene();

		static void SetClearColor(const glm::vec4& color);
		static void ClearBufferBit(u32 mask);

		static void EnableDepthTest();
		static void DisableDepthTest();

		static void EnableStencilTest();
		static void SetStencilPredicate(u32 predicate, i32 ref, u32 mask);
		static void EnableStencilMaskOverwrite();
		static void DisableStencilMaskOverwrite();

		// Draw
		static void Submit(SharedPtr<Shader> shader, SharedPtr<VertexArray> vertexArray, const glm::mat4& modelMatrix);
		static void SubmitArrays(SharedPtr<Shader> shader, SharedPtr<VertexArray> vertexArray, u32 first, u32 count, const glm::mat4& modelMatrix);
		static void SubmitArraysForScreen(SharedPtr<Shader> shader, SharedPtr<VertexArray> vertexArray, u32 first, u32 count);
		static void SubmitArraysForSkybox(SharedPtr<Shader> shader, SharedPtr<VertexArray> vertexArray, u32 first, u32 count, const glm::mat4& modelMatrix);
		static void SubmitModel(SharedPtr<Model> model, SharedPtr<Shader> shader, const glm::mat4& modelMatrix);

		static RendererAPI::EAPI GetAPI() { return RendererAPI::GetRendererAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewMatrix;
			glm::mat4 ProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};

}