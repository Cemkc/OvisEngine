#pragma once

#include "Ovis/Renderer/Buffer.h"
#include <memory>
#include <vector>

namespace Ovis
{
	class VertexArray
	{
	protected:
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) = 0;

		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer) = 0;
		inline std::shared_ptr<IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }

		static std::shared_ptr<VertexArray> Create();
	};
}