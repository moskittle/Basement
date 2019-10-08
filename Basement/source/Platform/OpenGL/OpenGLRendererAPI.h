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
		
		virtual void SetViewport(u32 x, u32 y, u32 width, u32 height) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		
		virtual void EnableDepthTest() override;
		virtual void DisableDepthTest() override;
		
		virtual void EnableStencilTest() override;
		virtual void SetStencilOp(u32 allFail, u32 depthFail, u32 allPass) override;
		virtual void SetStencilMask(u32 mask) override;
		virtual void SetStencilFunc(u32 func, i32 ref, u32 mask) override;
		
		virtual void DrawIndex(const SharedPtr<VertexArray>& vertexArray) override;
		virtual void DrawArrays(const SharedPtr<VertexArray>& vertexArray, u32 first, u32 count) override;
	};

}