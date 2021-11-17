#pragma once

#include "RendererAPI.h"

namespace Basement {

	class RenderCommand
	{
	public:
		static void Init() { s_RenderAPI->Init(); }
		static void SetViewport(u32 x, u32 y, u32 width, u32 height) { s_RenderAPI->SetViewport(x, y, width, height); }
		static void SetClearColor(const glm::vec4& color) { s_RenderAPI->SetClearColor(color); };
		static void Clear(u32 mask) { s_RenderAPI->Clear(mask); };

		static void EnableDepthTest() { s_RenderAPI->EnableDepthTest(); }
		static void DisableDepthTest() { s_RenderAPI->DisableDepthTest(); }
		static void SetDepthMask(bool mask) { s_RenderAPI->SetDepthMask(mask); }
		static void SetDepthFunc(u32 predicate) { s_RenderAPI->SetDepthFunc(predicate); }

		static void EnableStencilTest() { s_RenderAPI->EnableStencilTest(); }
		static void SetStencilOp(u32 allFail, u32 depthFail, u32 allPass) { s_RenderAPI->SetStencilOp(allFail, depthFail, allPass); }
		static void SetStencilOpSeperate(u32 face, u32 allFail, u32 depthFail, u32 allPass) { s_RenderAPI->SetStencilOpSeperate(face, allFail, depthFail, allPass); }
		static void SetStencilMask(u32 mask) { s_RenderAPI->SetStencilMask(mask); }
		static void SetStencilFunc(u32 predicate, i32 ref, u32 mask) { s_RenderAPI->SetStencilFunc(predicate, ref, mask); }

		static void DrawIndex(SharedPtr<VertexArray> vertexArray) { s_RenderAPI->DrawIndex(vertexArray); }
		static void DrawIndexWithPoints(SharedPtr<VertexArray> vertexArray) { s_RenderAPI->DrawIndexWithPoints(vertexArray); }
		static void DrawIndexWithLines(SharedPtr<VertexArray> vertexArray) { s_RenderAPI->DrawIndexWithLines(vertexArray); }
		static void DrawArrays(u32 first, u32 count) { s_RenderAPI->DrawArrays(first, count); }
		static void DrawArraysWithPoints(u32 first, u32 count) { s_RenderAPI->DrawArraysWithPoints(first, count); }
		static void DrawArraysWithLines(u32 first, u32 count) { s_RenderAPI->DrawArraysWithLines(first, count); }
	private:
		static RendererAPI* s_RenderAPI;
	};

}