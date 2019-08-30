#include "bmpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Basement {

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::EAPI::None:
			BM_CORE_ASSERT(false, "RendererAPI::None is currently supported")
			return nullptr;
		case RendererAPI::EAPI::OpenGL:
			return new OpenGLVertexArray();
		default:
			BM_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
	}
}