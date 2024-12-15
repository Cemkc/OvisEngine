#pragma once

#include "RenderCommand.h"
#include "Camera.h"

namespace Ovis
{
	class Renderer 
	{
	private:
		static glm::mat4 m_View;
		static glm::mat4 m_Projection;
	public:
		static void BeginScene(const std::shared_ptr<Camera> camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}
