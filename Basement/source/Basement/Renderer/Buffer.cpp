#include "bmpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Basement {

	//----------------------------------------------------------
	// --Buffer Element-----------------------------------------
	BufferElement::BufferElement(EShaderDataType type, const std::string& name, bool isNormalized) :
		Name(name),
		Type(type),
		Size(GetShaderDataTypeSize(type)),
		Offset(0),
		bIsNormalized(isNormalized)
	{
	}

	//---------------------------------------------------------
	// --Buffer Layout-----------------------------------------
	BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& initList) : m_BufferElements(initList)
	{
		CalculateOffsetAndStride();
	}

	void BufferLayout::CalculateOffsetAndStride()
	{
		u32 offset = 0;
		for (auto& element : m_BufferElements)
		{
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}

	//---------------------------------------------------------
	// --Vertex Buffer-----------------------------------------
	SharedPtr<VertexBuffer> VertexBuffer::Create(u32 size, float* vertices)
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

	SharedPtr<VertexBuffer> VertexBuffer::Create(const std::vector<Vertex>& vertices)
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

	//--------------------------------------------------------
	// --Index Buffer-----------------------------------------
	SharedPtr<IndexBuffer> IndexBuffer::Create(u32 count, u32* indices)
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

	SharedPtr<IndexBuffer> IndexBuffer::Create(const std::vector<u32>& indices)
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


	//----------------------------------------------------------
	//--Uniform Buffer------------------------------------------
	SharedPtr<UniformBuffer> UniformBuffer::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::EAPI::None:
			BM_CORE_ASSERT(false, "RendererAPI::None is currently supported")
				return nullptr;
		case RendererAPI::EAPI::OpenGL:
			return std::make_shared<OpenGLUniformBuffer>();
		default:
			BM_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
	}

}