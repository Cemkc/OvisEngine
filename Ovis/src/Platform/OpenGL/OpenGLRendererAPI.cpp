#include "ovpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h> 

namespace Ovis
{
	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	void Ovis::OpenGLRendererAPI::SetClearColor(glm::uvec4 color)
	{
		glClearColor(color.r, color.b, color.g, color.a);
	}

	void Ovis::OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}
