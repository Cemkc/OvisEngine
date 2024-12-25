#pragma once

#include "RenderCommand.h"
#include "Camera.h"
#include "Ovis/GameEntity/Transform.h"

namespace Ovis
{
	class Renderer 
	{
	private:
		struct SceneData
		{
			glm::mat4 ViewMatrix;
			glm::mat4 ProjectionMatrix;
		};

		static std::unique_ptr<SceneData> s_SceneData;
	public:
		static void BeginScene(const Camera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const Transform transform);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}
