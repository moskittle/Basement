#pragma once

#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"

namespace Basement {
	
	class Renderer 
	{
	public:
		static void Init();
		static void ResizeWindow(u32 width, u32 height);

		static void BeginScene(Camera3D& camera);
		static void BeginScene(Camera2D& camera);
		static void EndScene();

		static void Submit(const SharedPtr<Shader>& shader, const SharedPtr<VertexArray>& vertexArray, const glm::mat4& modelMatrix);
		static void SubmitArrays(const SharedPtr<Shader>& shader, const SharedPtr<VertexArray>& vertexArray, u32 first, u32 count, const glm::mat4& modelMatrix);

		inline static RendererAPI::EAPI GetAPI() { return RendererAPI::GetRendererAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewMatrix;
			glm::mat4 ProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};

}