#pragma once

#include "RendererAPI.h"

namespace Basement {

	class RenderCommand
	{
	public:
		static void Init() { s_RenderAPI->Init(); }
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { s_RenderAPI->SetViewport(x, y, width, height); }
		static void SetClearColor(const glm::vec4& color) { s_RenderAPI->SetClearColor(color); };
		static void Clear() { s_RenderAPI->Clear(); };
		static void EnableDepthTest() { s_RenderAPI->EnableDepthTest(); }
		static void DrawIndex(const Shared<VertexArray>& vertexArray) { s_RenderAPI->DrawIndex(vertexArray); }
		static void DrawArrays(const Shared<VertexArray>& vertexArray, uint32_t first, uint32_t count) { s_RenderAPI->DrawArrays(vertexArray, first, count); }
	private:
		static RendererAPI* s_RenderAPI;
	};

}