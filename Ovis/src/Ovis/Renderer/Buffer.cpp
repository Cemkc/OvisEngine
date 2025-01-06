#include "ovpch.h"

#include "Renderer.h"
#include "Buffer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Ovis
{
	std::shared_ptr<VertexBuffer> Ovis::VertexBuffer::Create(float* vertices, uint32_t size)
	{
		OV_RENDER_PROFILE_FUNC();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:    OV_CORE_ASSERT(false, "No Rendering API specification provided!"); return nullptr;
		case RendererAPI::OpenGL:  return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		OV_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		OV_RENDER_PROFILE_FUNC();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:    OV_CORE_ASSERT(false, "No Rendering API specification provided!"); return nullptr;
		case RendererAPI::OpenGL:  return std::make_shared<OpenGLIndexBuffer>(indices, count);
		}

		OV_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}

