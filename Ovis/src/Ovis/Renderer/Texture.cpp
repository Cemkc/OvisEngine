#include "ovpch.h"

#include "Texture.h"

#include "Ovis/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Ovis
{
	std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:    OV_CORE_ASSERT(false, "No Rendering API specification provided!"); return nullptr;
		case RendererAPI::OpenGL:  return std::make_shared<OpenGLTexture2D>(path);
		}

		OV_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}