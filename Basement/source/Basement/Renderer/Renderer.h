#pragma once

#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"

namespace Basement {
	
	class Renderer 
	{
	public:
		static void Init();
		static void ResizeWindow(uint32_t width, uint32_t height);

		static void BeginScene(Camera3D& camera);
		static void BeginScene(Camera2D& camera);
		static void EndScene();

		static void Submit(const Shared<Shader>& shader, const Shared<VertexArray>& vertexArray, const glm::mat4& modelMatrix);
		static void SubmitArrays(const Shared<Shader>& shader, const Shared<VertexArray>& vertexArray, uint32_t first, uint32_t count, const glm::mat4& modelMatrix);

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