#pragma once

namespace Ovis
{
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Mat3, Mat4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:   return 4;
		case ShaderDataType::Float2:  return 4 * 2;
		case ShaderDataType::Float3:  return 4 * 3;
		case ShaderDataType::Float4:  return 4 * 4;
		case ShaderDataType::Int:     return 4;
		case ShaderDataType::Int2:    return 4 * 2;
		case ShaderDataType::Int3:	  return 4 * 3;
		case ShaderDataType::Int4:	  return 4 * 4;
		case ShaderDataType::Mat3:    return 4 * 3 * 3;
		case ShaderDataType::Mat4:    return 4 * 4 * 4;
		case ShaderDataType::Bool:    return 1;
		}

		OV_CORE_ASSERT(false, "Specified buffer type does not fall within the defined buffer types!");
		return 0;
	}

	struct BufferAttribute
	{
	public:
		ShaderDataType Type;
		std::string Name;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		BufferAttribute(ShaderDataType bufferType, std::string name, bool normalized = false)
			:Type(bufferType), Name(name), Size(ShaderDataTypeSize(bufferType)), Offset(0), Normalized(normalized)
		{

		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Mat3:    return 3 * 3;
			case ShaderDataType::Mat4:    return 4 * 4;
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Bool:    return 1;
			}

			OV_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout
	{
	private:
		std::vector<BufferAttribute> m_BufferAttributes;
		uint32_t m_Stride;
	public:
		std::vector<BufferAttribute>::iterator begin() { return m_BufferAttributes.begin(); }
		std::vector<BufferAttribute>::iterator end() { return m_BufferAttributes.end(); }
		std::vector<BufferAttribute>::const_iterator begin() const { return m_BufferAttributes.begin(); }
		std::vector<BufferAttribute>::const_iterator end() const { return m_BufferAttributes.end(); }

		BufferLayout() {}

		BufferLayout(std::initializer_list<BufferAttribute> bufferAttribs)
			: m_BufferAttributes(bufferAttribs)
		{
			int offset = 0;
			m_Stride = 0;
			for (BufferAttribute& attribute : m_BufferAttributes)
			{
				attribute.Offset = offset;
				offset += attribute.Size;
				m_Stride += attribute.Size;
			}
		}

		inline const std::vector<BufferAttribute>& GetAttributes() const { return m_BufferAttributes; }
		uint32_t GetSrtide() const { return m_Stride; }
	};

	class VertexBuffer
	{
	protected:
		BufferLayout m_Layout;
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void SetData(const void* data, uint32_t size) const = 0;

		inline void SetLayout(const BufferLayout& layout) { m_Layout = layout; };
		inline const BufferLayout& GetLayout() const { return m_Layout; };


		static std::shared_ptr<VertexBuffer> Create(uint32_t size);
		static std::shared_ptr<VertexBuffer> Create(float* vertices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static std::shared_ptr<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};
	
}