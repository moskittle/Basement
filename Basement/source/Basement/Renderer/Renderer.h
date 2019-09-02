#pragma once

#include "RenderCommand.h"
#include "Camera.h"
#include "ShaderProgram.h"

namespace Basement {
	
	class Renderer 
	{
	public:
		static void BeginScene(Camera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<ShaderProgram>& shaderProgram, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& modelMatrix);

		inline static RendererAPI::EAPI GetAPI() { return RendererAPI::GetRendererAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};

}