#pragma once

#include "Basement/Renderer/Buffer.h"

namespace Basement {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size, float* vertices);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		uint32_t m_VertexBufferID;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t count, uint32_t* indices);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual inline uint32_t GetCount() const override { return m_Count; }
		virtual inline uint32_t GetRendererID() const override { return m_IndexBufferID; }
	private:
		uint32_t m_IndexBufferID;
		uint32_t m_Count;
	};


}