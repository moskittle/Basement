#pragma once

namespace Basement {

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static VertexBuffer* Create(uint32_t size, float* vertices);

	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual inline uint32_t GetCount() const = 0;
		virtual inline uint32_t GetRendererID() const = 0;

		static IndexBuffer* Create(uint32_t count, uint32_t* indices);
	};

}