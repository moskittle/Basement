#include "bmpch.h"
#include "OpenGLRendererAPI.h"
#include "OpenGLDebug.h"

namespace Basement {

	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		RendererAPIInfo& info = RendererAPI::GetRendererAPIInfo();
		info.Vendor = (const char*)glGetString(GL_VENDOR);
		info.Renderer = (const char*)glGetString(GL_RENDERER);
		info.Version = (const char*)glGetString(GL_VERSION);

		OpenGLCall(;) // Check init errors
	}

	void OpenGLRendererAPI::SetViewport(u32 x, u32 y, u32 width, u32 height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear(u32 mask)
	{
		glClear(mask);
	}

	void OpenGLRendererAPI::EnableDepthTest()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::DisableDepthTest()
	{
		glDisable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetDepthMask(bool mask)
	{
		glDepthMask(mask);
	}

	void OpenGLRendererAPI::SetDepthFunc(u32 predicate)
	{
		glDepthFunc(predicate);
	}

	void OpenGLRendererAPI::EnableStencilTest()
	{
		glEnable(GL_STENCIL_TEST);
	}

	void OpenGLRendererAPI::SetStencilOp(u32 allFail, u32 depthFail, u32 allPass)
	{
		glStencilOp(allFail, depthFail, allPass);
	}

	void OpenGLRendererAPI::SetStencilOpSeperate(u32 face, u32 allFail, u32 depthFail, u32 allPass)
	{
		glStencilOpSeparate(face, allFail, depthFail, allPass);
	}

	void OpenGLRendererAPI::SetStencilMask(u32 mask)
	{
		glStencilMask(mask);
	}

	void OpenGLRendererAPI::SetStencilFunc(u32 predicate, i32 ref, u32 mask)
	{
		glStencilFunc(predicate, ref, mask);
	}

	void OpenGLRendererAPI::DrawIndex(SharedPtr<VertexArray> vertexArray)
	{
		OpenGLCall(glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void OpenGLRendererAPI::DrawIndexWithPoints(SharedPtr<VertexArray> vertexArray)
	{
		OpenGLCall(glDrawElements(GL_POINTS, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void OpenGLRendererAPI::DrawIndexWithLines(SharedPtr<VertexArray> vertexArray)
	{
		OpenGLCall(glDrawElements(GL_LINES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void OpenGLRendererAPI::DrawArrays(u32 first, u32 count)
	{
		OpenGLCall(glDrawArrays(GL_TRIANGLES, first, count));
	}

}