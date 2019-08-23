#include "bmpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Basement {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case ERendererAPI::None:
			BM_CORE_ASSERT(false, "RendererAPI::None is currently supported")
			return nullptr;
		case ERendererAPI::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
		default:
			BM_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case ERendererAPI::None:
			BM_CORE_ASSERT(false, "RendererAPI::None is currently supported")
				return nullptr;
		case ERendererAPI::OpenGL:
			return new OpenGLIndexBuffer(indices, count);
		default:
			BM_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
	}

}