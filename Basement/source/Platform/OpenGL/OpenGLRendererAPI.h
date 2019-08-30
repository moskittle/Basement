#pragma once

#include "Basement/Renderer/RendererAPI.h"

namespace Basement {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		OpenGLRendererAPI() = default;
		~OpenGLRendererAPI() = default;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void DrawIndex(const std::shared_ptr<VertexArray>& vertexArray) override;
	};

}