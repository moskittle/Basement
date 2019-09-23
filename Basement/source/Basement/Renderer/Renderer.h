#pragma once

#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"

namespace Basement {
	
	class Renderer 
	{
	public:
		static void Init();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Shared<Shader>& shader, const Shared<VertexArray>& vertexArray, const glm::mat4& modelMatrix);

		inline static RendererAPI::EAPI GetAPI() { return RendererAPI::GetRendererAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};

}