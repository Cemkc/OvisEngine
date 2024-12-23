#include "ovpch.h"

#include "Ovis/Core/Log.h"
#include "OpenGLVertexArray.h"
#include "glad/glad.h"

namespace Ovis
{

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Ovis::ShaderDataType::Float:    return GL_FLOAT;
		case Ovis::ShaderDataType::Float2:   return GL_FLOAT;
		case Ovis::ShaderDataType::Float3:   return GL_FLOAT;
		case Ovis::ShaderDataType::Float4:   return GL_FLOAT;
		case Ovis::ShaderDataType::Mat3:     return GL_FLOAT;
		case Ovis::ShaderDataType::Mat4:     return GL_FLOAT;
		case Ovis::ShaderDataType::Int:      return GL_INT;
		case Ovis::ShaderDataType::Int2:     return GL_INT;
		case Ovis::ShaderDataType::Int3:     return GL_INT;
		case Ovis::ShaderDataType::Int4:     return GL_INT;
		case Ovis::ShaderDataType::Bool:     return GL_BOOL;
		}

		OV_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_RendererId);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererId);
	}

	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

	void Ovis::OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		OV_CORE_ASSERT(vertexBuffer->GetLayout().GetAttributes().size(), "Buffer layout is not defined!");
		glBindVertexArray(m_RendererId);
		vertexBuffer->Bind();

		const auto& bufferLayout = vertexBuffer->GetLayout();
		int index = 0;
		for (const auto& attribute : bufferLayout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, attribute.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(attribute.Type),
				attribute.Normalized ? GL_TRUE : GL_FALSE,
				bufferLayout.GetSrtide(),
				(void*)attribute.Offset);

			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererId);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}