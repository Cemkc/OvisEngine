#pragma once

#include "Ovis/Renderer/Buffer.h"

namespace Ovis 
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	private:
		uint32_t m_RendererId;
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer() override;

		virtual void SetData(uint32_t size, const void* data) const override;

		virtual void Bind() const override;
		virtual void UnBind() const override;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	private:
		uint32_t m_RendererId;
		uint32_t m_IndexCount;
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer() override;

		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual uint32_t GetCount() const override { return m_IndexCount; }
	};
}

