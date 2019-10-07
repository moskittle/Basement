#pragma once

#include <vector>
#include <memory>

#include "Basement/Renderer/VertexArray.h"

namespace Basement {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const SharedPtr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const SharedPtr<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<SharedPtr<VertexBuffer>>& GetVertexBuffers() const override {	return m_VertexBuffers;	}
		virtual const SharedPtr<IndexBuffer>& GetIndexBuffer() const override{ return m_IndexBuffer; }
	private:
		u32 m_VertexArrayID;
		u32 m_VertexArrayIndexOffset = 0;
		std::vector<SharedPtr<VertexBuffer>> m_VertexBuffers;
		SharedPtr<IndexBuffer> m_IndexBuffer;
	};

}