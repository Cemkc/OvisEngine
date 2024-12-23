#pragma once

#include "RenderCommand.h"
#include "Camera.h"
#include "Ovis/GameEntity/Transform.h"

namespace Ovis
{
	class Renderer 
	{
	private:
		static glm::mat4 m_View;
		static glm::mat4 m_Projection;
	public:
		static void BeginScene(const Camera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const Transform transform);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}
