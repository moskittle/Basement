#include "bmpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Basement {

	// --Buffer Element-----------------------------------------
	BufferElement::BufferElement(EShaderDataType type, const std::string& name, bool isNormalized)
		: Name(name), Type(type), Size(GetShaderDataTypeSize(type)), Offset(0), bIsNormalized(isNormalized)
	{
	}


	// --Buffer Layout-----------------------------------------
	BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& initList)
		: m_BufferElements(initList)
	{
		CalculateOffsetAndStride();
	}

	void BufferLayout::CalculateOffsetAndStride()
	{
		uint32_t offset = 0;
		for (auto& element : m_BufferElements)
		{
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}


	// --Vertex Buffer-----------------------------------------
	Shared<VertexBuffer> VertexBuffer::Create(uint32_t size, float* vertices)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::EAPI::None:
				BM_CORE_ASSERT(false, "RendererAPI::None is currently supported")
				return nullptr;
			case RendererAPI::EAPI::OpenGL:
				return std::make_shared<OpenGLVertexBuffer>(size, vertices);
			default:
				BM_CORE_ASSERT(false, "Unknown RendererAPI");
				return nullptr;
		}
	}

	Shared<VertexBuffer> VertexBuffer::Create(const std::vector<Vertex>& vertices)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::EAPI::None:
				BM_CORE_ASSERT(false, "RendererAPI::None is currently supported")
					return nullptr;
			case RendererAPI::EAPI::OpenGL:
				return std::make_shared<OpenGLVertexBuffer>(vertices);
			default:
				BM_CORE_ASSERT(false, "Unknown RendererAPI");
				return nullptr;
		}
	}


	// --Index Buffer-----------------------------------------
	Shared<IndexBuffer> IndexBuffer::Create(uint32_t count, uint32_t* indices)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::EAPI::None:
				BM_CORE_ASSERT(false, "RendererAPI::None is currently supported")
				return nullptr;
			case RendererAPI::EAPI::OpenGL:
				return std::make_shared<OpenGLIndexBuffer>(count, indices);
			default:
				BM_CORE_ASSERT(false, "Unknown RendererAPI");
				return nullptr;
		}
	}

	Shared<IndexBuffer> IndexBuffer::Create(const std::vector<uint32_t>& indices)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::EAPI::None:
				BM_CORE_ASSERT(false, "RendererAPI::None is currently supported")
					return nullptr;
			case RendererAPI::EAPI::OpenGL:
				return std::make_shared<OpenGLIndexBuffer>(indices);
			default:
				BM_CORE_ASSERT(false, "Unknown RendererAPI");
				return nullptr;
		}
	}

}