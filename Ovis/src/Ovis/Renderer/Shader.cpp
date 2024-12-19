#include "ovpch.h"
#include "Shader.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Ovis
{
	Shader* Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:    OV_CORE_ASSERT(false, "No Rendering API specification provided!"); return nullptr;
			case RendererAPI::OpenGL:  return new OpenGLShader(filepath);
		}

		OV_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Shader* Shader::Create(const std::string& vertexScr, const std::string& fragmentScr)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:    OV_CORE_ASSERT(false, "No Rendering API specification provided!"); return nullptr;
		case RendererAPI::OpenGL:  return new OpenGLShader(vertexScr, fragmentScr);
		}

		OV_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}