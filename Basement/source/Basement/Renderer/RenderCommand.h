#pragma once

#include "RendererAPI.h"

namespace Basement {

	class RenderCommand
	{
	public:
		inline static void Init() { s_RenderAPI->Init(); }
		inline static void SetClearColor(const glm::vec4& color) { s_RenderAPI->SetClearColor(color); };
		inline static void Clear() { s_RenderAPI->Clear(); };
		inline static void DrawIndex(const Shared<VertexArray>& vertexArray) { s_RenderAPI->DrawIndex(vertexArray); }
	private:
		static RendererAPI* s_RenderAPI;
	};

}