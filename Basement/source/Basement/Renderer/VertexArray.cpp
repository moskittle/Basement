#include "bmpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Basement {

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case ERendererAPI::None:
			BM_CORE_ASSERT(false, "RendererAPI::None is currently supported")
			return nullptr;
		case ERendererAPI::OpenGL:
			return new OpenGLVertexArray();
		default:
			BM_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
	}
}