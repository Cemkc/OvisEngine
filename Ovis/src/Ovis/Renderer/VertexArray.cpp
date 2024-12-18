#include "ovpch.h"

#include "VertexArray.h"
#include "Ovis/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Ovis
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:    OV_CORE_ASSERT(false, "No Rendering API specification provided!"); return nullptr;
		case RendererAPI::OpenGL:  return new OpenGLVertexArray();
		}

		OV_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}