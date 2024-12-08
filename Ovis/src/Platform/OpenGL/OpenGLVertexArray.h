#pragma once

#include "Ovis/Renderer/VertexArray.h"

namespace Ovis
{
	class OpenGLVertexArray : public VertexArray
	{
	private:
		uint32_t m_RendererId;
	public:
		OpenGLVertexArray();

		void Bind() const override;
		void UnBind() const override;
		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) override;
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer) override;
	};
}