#pragma once

namespace Basement {

	enum class EShaderDataType	// TODO: uint8 for packing
	{
		None = 0, 
		Int, Int2, Int3, Int4, 
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Bool
	};

	static uint32_t GetShaderDataTypeSize(EShaderDataType type)
	{
		switch (type)
		{
			case EShaderDataType::Int:     return 4;
			case EShaderDataType::Int2:    return 4 * 2;
			case EShaderDataType::Int3:    return 4 * 3;
			case EShaderDataType::Int4:    return 4 * 4;
			case EShaderDataType::Float:   return 4;
			case EShaderDataType::Float2:  return 4 * 2;
			case EShaderDataType::Float3:  return 4 * 3;
			case EShaderDataType::Float4:  return 4 * 4;
			case EShaderDataType::Mat3:    return 4 * 3 * 3;
			case EShaderDataType::Mat4:    return 4 * 4 * 4;
			case EShaderDataType::Bool:    return 1;
		}

		BM_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		EShaderDataType Type;
		uint32_t Offset;
		uint32_t Size;
		bool bIsNormalized;

		BufferElement(EShaderDataType type, const std::string& name, bool isNormalized = false);
		~BufferElement() = default;

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case EShaderDataType::Int:      return 1;
			case EShaderDataType::Int2:     return 2;
			case EShaderDataType::Int3:     return 3;
			case EShaderDataType::Int4:     return 4;
			case EShaderDataType::Float:    return 1;
			case EShaderDataType::Float2:   return 2;
			case EShaderDataType::Float3:   return 3;
			case EShaderDataType::Float4:   return 4;
			case EShaderDataType::Mat3:     return 3;
			case EShaderDataType::Mat4:     return 4;
			case EShaderDataType::Bool:     return 1;
			}

			BM_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};


	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& initList);
		~BufferLayout() = default;

		inline const std::vector<BufferElement> GetElements() const { return m_BufferElements; }
		inline uint32_t GetStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_BufferElements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_BufferElements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_BufferElements.cbegin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_BufferElements.cend(); }
	private:
		void CalculateOffsetAndStride();
	private:
		std::vector<BufferElement> m_BufferElements;
		uint32_t m_Stride = 0;
	};


	class VertexBuffer
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		
		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static VertexBuffer* Create(uint32_t size, float* vertices);
	};


	class IndexBuffer
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual inline uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t count, uint32_t* indices);
	};

}