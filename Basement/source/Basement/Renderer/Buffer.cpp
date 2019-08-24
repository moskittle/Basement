#include "bmpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Basement {

	VertexBuffer* VertexBuffer::Create(uint32_t size, float* vertices)
	{
		switch (Renderer::GetAPI())
		{
		case ERendererAPI::None:
			BM_CORE_ASSERT(false, "RendererAPI::None is currently supported")
			return nullptr;
		case ERendererAPI::OpenGL:
			return new OpenGLVertexBuffer(size, vertices);
		default:
			BM_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
	}

	IndexBuffer* IndexBuffer::Create(uint32_t count, uint32_t* indices)
	{
		switch (Renderer::GetAPI())
		{
		case ERendererAPI::None:
			BM_CORE_ASSERT(false, "RendererAPI::None is currently supported")
				return nullptr;
		case ERendererAPI::OpenGL:
			return new OpenGLIndexBuffer(count, indices);
		default:
			BM_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
	}

}