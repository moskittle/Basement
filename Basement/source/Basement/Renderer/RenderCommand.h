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
		static void DisableDepthTest() { s_RenderAPI->DisableDepthTest(); }
		
		static void EnableStencilTest() { s_RenderAPI->EnableStencilTest(); }
		static void SetStencilOp(u32 allFail, u32 depthFail, u32 allPass) { s_RenderAPI->SetStencilOp(allFail, depthFail, allPass); }
		static void SetStencilMask(u32 mask) { s_RenderAPI->SetStencilMask(mask); }
		static void SetStenceilFunc(u32 func, i32 ref, u32 mask) { s_RenderAPI->SetStencilFunc(func, ref, mask); }
		
		static void DrawIndex(const SharedPtr<VertexArray>& vertexArray) { s_RenderAPI->DrawIndex(vertexArray); }
		static void DrawArrays(const SharedPtr<VertexArray>& vertexArray, u32 first, u32 count) { s_RenderAPI->DrawArrays(vertexArray, first, count); }
	private:
		static RendererAPI* s_RenderAPI;
	};

}