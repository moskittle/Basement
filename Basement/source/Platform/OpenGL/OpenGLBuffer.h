#pragma once

#include "Basement/Renderer/Buffer.h"

namespace Basement {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(u32 size, float* vertices);
		OpenGLVertexBuffer(const std::vector<Vertex>& vertices);
		~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const BufferLayout& GetLayout() const override { return m_BufferLayout; }
		virtual void SetLayout(const BufferLayout& bufferLayout) override { m_BufferLayout = bufferLayout; }

		virtual u32 GetCount() const override { return m_Count; }
	private:
		u32 m_VertexBufferID;
		BufferLayout m_BufferLayout;
		u32 m_Count;
	};


	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(u32 count, u32* indices);
		OpenGLIndexBuffer(const std::vector<u32>& indices);
		~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual u32 GetCount() const override { return m_Count; }
	private:
		u32 m_IndexBufferID;
		u32 m_Count;
	};

}