#pragma once

#include "RendererAPI.h"

namespace Basement {

	class RenderCommand
	{
	public:
		static void Init() { s_RenderAPI->Init(); }
		static void SetViewport(u32 x, u32 y, u32 width, u32 height) { s_RenderAPI->SetViewport(x, y, width, height); }
		static void SetClearColor(const glm::vec4& color) { s_RenderAPI->SetClearColor(color); };
		static void Clear() { s_RenderAPI->Clear(); };
		static void EnableDepthTest() { s_RenderAPI->EnableDepthTest(); }
		static void DrawIndex(const SharedPtr<VertexArray>& vertexArray) { s_RenderAPI->DrawIndex(vertexArray); }
		static void DrawArrays(const SharedPtr<VertexArray>& vertexArray, u32 first, u32 count) { s_RenderAPI->DrawArrays(vertexArray, first, count); }
	private:
		static RendererAPI* s_RenderAPI;
	};

}