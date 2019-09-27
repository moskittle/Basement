#include "bmpch.h"
#include "OpenGLRendererAPI.h"
#include "OpenGLDebug.h"

namespace Basement {
	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		RendererAPIInfo& info = RendererAPI::GetRendererAPIInfo();
		info.Vendor = (const char*) glGetString(GL_VENDOR);
		info.Renderer = (const char*) glGetString(GL_RENDERER);
		info.Version= (const char*) glGetString(GL_VERSION);
		
		OpenGLCall() // Check errors
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::EnableDepthTest()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::DrawIndex(const Shared<VertexArray>& vertexArray)
	{
		OpenGLCall(glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void OpenGLRendererAPI::DrawArrays(const Shared<VertexArray>& vertexArray, uint32_t first, uint32_t count)
	{
		OpenGLCall(glDrawArrays(GL_TRIANGLES, first, count));
	}

}