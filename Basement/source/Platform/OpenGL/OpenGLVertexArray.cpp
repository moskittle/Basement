#include "bmpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Basement {

	// TODO move to class as Getter()
	static GLenum ShaderDataTypeToOpenGLBaseType(EShaderDataType type)
	{
		switch (type)
		{
			case EShaderDataType::Int:      return GL_INT;
			case EShaderDataType::Int2:     return GL_INT;
			case EShaderDataType::Int3:     return GL_INT;
			case EShaderDataType::Int4:     return GL_INT;
			case EShaderDataType::Float:    return GL_FLOAT;
			case EShaderDataType::Float2:   return GL_FLOAT;
			case EShaderDataType::Float3:   return GL_FLOAT;
			case EShaderDataType::Float4:   return GL_FLOAT;
			case EShaderDataType::Mat3:     return GL_FLOAT;
			case EShaderDataType::Mat4:     return GL_FLOAT;
			case EShaderDataType::Bool:     return GL_BOOL;
		}

		BM_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}


	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_VertexArrayID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_VertexArrayID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_VertexArrayID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Shared<VertexBuffer>& vertexBuffer)
	{
		BM_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");
		
		glBindVertexArray(m_VertexArrayID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const BufferLayout& bufferLayout = vertexBuffer->GetLayout();
		for (const auto& element : bufferLayout)
		{
			// Set vertex attributes
			glEnableVertexAttribArray(index + m_VertexArrayIndexOffset);
			glVertexAttribPointer(index + m_VertexArrayIndexOffset,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.bIsNormalized ? GL_TRUE : GL_FALSE,
				vertexBuffer->GetLayout().GetStride(),
				(const void*)(intptr_t)element.Offset);

			++index;
		}

		m_VertexBuffers.push_back(vertexBuffer);
		m_VertexArrayIndexOffset += static_cast<uint32_t>(bufferLayout.GetElements().size());
	}

	void OpenGLVertexArray::SetIndexBuffer(const Shared<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_VertexArrayID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}

}