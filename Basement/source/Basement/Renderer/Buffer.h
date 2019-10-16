#pragma once

#include <glm/glm.hpp>

namespace Basement {

	enum class EShaderDataType	// TODO: uint8 for packing
	{
		None = 0, 
		Int, Int2, Int3, Int4, 
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Bool
	};

	static u32 GetShaderDataTypeSize(EShaderDataType type)
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
		u32 Offset;
		u32 Size;
		bool bIsNormalized;

		BufferElement(EShaderDataType type, const std::string& name, bool isNormalized = false);
		~BufferElement() = default;

		u32 GetComponentCount() const
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
		inline u32 GetStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_BufferElements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_BufferElements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_BufferElements.cbegin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_BufferElements.cend(); }
	private:
		void CalculateOffsetAndStride();
	private:
		std::vector<BufferElement> m_BufferElements;
		u32 m_Stride = 0;
	};

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoord;
	};

	class VertexBuffer
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		
		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
		
		virtual u32 GetCount() const = 0;

		static SharedPtr<VertexBuffer> Create(u32 size, float* vertices);
		static SharedPtr<VertexBuffer> Create(const std::vector<Vertex>& vertices);
	};


	class IndexBuffer
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual inline u32 GetCount() const = 0;

		static SharedPtr<IndexBuffer> Create(u32 count, u32* indices);
		static SharedPtr<IndexBuffer> Create(const std::vector<u32>& indices);
	};


	class UniformBuffer
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
		
		//virtual void GetIndex() = 0;

		static SharedPtr<UniformBuffer> Create();
	};
}