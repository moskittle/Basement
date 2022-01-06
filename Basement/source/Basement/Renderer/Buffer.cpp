#include "bmpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLDebug.h"

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

	//----------------------------------------------------------
	//--Uniform Buffer------------------------------------------
	SharedPtr<FrameBuffer> FrameBuffer::Create(u32 width, u32 height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::EAPI::None:
			BM_CORE_ASSERT(false, "RendererAPI::None is currently supported")
				return nullptr;
		case RendererAPI::EAPI::OpenGL:
			return std::make_shared<OpenGLFrameBuffer>(width, height);
		default:
			BM_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
	}

	//----------------------------------------------------------
	//--Frame Buffer------------------------------------------
	OpenGLFrameBuffer::OpenGLFrameBuffer(u32 width, u32 height)
	{
		// Create frambuffer
		OpenGLCall(glGenFramebuffers(1, &m_FrameBufferID));
		OpenGLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID));

		// Create attachments
		CreateColorAttachment(width, height);
		CreateDepthAttechment(width, height);

		// Check framebuffer status
		BM_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer incomplete!");

		OpenGLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	}

	void OpenGLFrameBuffer::CreateColorAttachment(u32 width, u32 height)
	{
		OpenGLCall(glGenTextures(1, &m_ColorAttachmentID));
		OpenGLCall(glBindTexture(GL_TEXTURE_2D, m_ColorAttachmentID));

		OpenGLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
		OpenGLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		OpenGLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		OpenGLCall(glBindTexture(GL_TEXTURE_2D, 0));

		OpenGLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachmentID, 0));
	}

	void OpenGLFrameBuffer::CreateDepthAttechment(u32 width, u32 height)
	{
		OpenGLCall(glGenRenderbuffers(1, &m_DepthAttachmentID));
		OpenGLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachmentID));
		OpenGLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
		OpenGLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));

		OpenGLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachmentID));
	}

	void OpenGLFrameBuffer::Bind() const
	{
		OpenGLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID));
	}

	void OpenGLFrameBuffer::Unbind() const
	{
		OpenGLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void OpenGLFrameBuffer::ActivateTexture(u32 slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		OpenGLCall(glBindTexture(GL_TEXTURE_2D, m_ColorAttachmentID));
	}

}