#pragma once

#include "Basement/Renderer/Buffer.h"

namespace Basement {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size, float* vertices);
		~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const BufferLayout& GetLayout() const override { return m_BufferLayout; }
		virtual void SetLayout(const BufferLayout& bufferLayout) override { m_BufferLayout = bufferLayout; }

		virtual uint32_t GetCount() const override { return m_Count; }
	private:
		uint32_t m_VertexBufferID;
		BufferLayout m_BufferLayout;
		uint32_t m_Count;
	};


	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t count, uint32_t* indices);
		~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override { return m_Count; }
	private:
		uint32_t m_IndexBufferID;
		uint32_t m_Count;
	};

}