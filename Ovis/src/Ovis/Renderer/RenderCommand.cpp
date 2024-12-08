#include "ovpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Ovis
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}