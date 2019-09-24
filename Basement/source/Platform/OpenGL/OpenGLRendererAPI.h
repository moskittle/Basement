#pragma once

#include "Basement/Renderer/RendererAPI.h"

#include <glad/glad.h>

namespace Basement {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		OpenGLRendererAPI() = default;
		~OpenGLRendererAPI() = default;

		virtual void Init() override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void EnableDepthTest() override;
		virtual void DrawIndex(const Shared<VertexArray>& vertexArray) override;
		virtual void DrawArrays(const Shared<VertexArray>& vertexArray, uint32_t first, uint32_t count) override;
	};

}