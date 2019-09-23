#pragma once

#include "RendererAPI.h"

namespace Basement {

	class RenderCommand
	{
	public:
		static void Init() { s_RenderAPI->Init(); }
		static void SetClearColor(const glm::vec4& color) { s_RenderAPI->SetClearColor(color); };
		static void Clear() { s_RenderAPI->Clear(); };
		static void EnableDepthTest() { s_RenderAPI->EnableDepthTest(); }
		static void DrawIndex(const Shared<VertexArray>& vertexArray) { s_RenderAPI->DrawIndex(vertexArray); }
	private:
		static RendererAPI* s_RenderAPI;
	};

}