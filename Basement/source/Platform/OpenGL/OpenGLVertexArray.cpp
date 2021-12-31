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
		// May change for compatibility
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

	void OpenGLVertexArray::AddVertexBuffer(const SharedPtr<VertexBuffer>& vertexBuffer)
	{
		BM_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_VertexArrayID);
		vertexBuffer->Bind();

		u32 index = 0;
		const BufferLayout& bufferLayout = vertexBuffer->GetLayout();
		for (const auto& element : bufferLayout)
		{
			// Set vertex attributes
			glEnableVertexAttribArray(index + m_VertexArrayIndexOffset);
			bool isIntegerType = element.Type == EShaderDataType::Int || element.Type == EShaderDataType::Int2 ||
				element.Type == EShaderDataType::Int3 || element.Type == EShaderDataType::Int4;

			if (isIntegerType)
			{
				glVertexAttribIPointer
				(
					index + m_VertexArrayIndexOffset,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					vertexBuffer->GetLayout().GetStride(),
					(const void*)(intptr_t)element.Offset
				);
			}
			else
			{
				glVertexAttribPointer
				(
					index + m_VertexArrayIndexOffset,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.bIsNormalized ? GL_TRUE : GL_FALSE,
					vertexBuffer->GetLayout().GetStride(),
					(const void*)(intptr_t)element.Offset
				);
			}

			++index;
		}

		m_VertexBuffers.push_back(vertexBuffer);
		m_VertexArrayIndexOffset += static_cast<u32>(bufferLayout.GetElements().size());
	}

	void OpenGLVertexArray::SetIndexBuffer(const SharedPtr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_VertexArrayID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}

}