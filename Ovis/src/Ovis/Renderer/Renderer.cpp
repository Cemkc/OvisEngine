#include "ovpch.h"
#include "Renderer.h"

#include <glad/glad.h>
#include "Ovis/Core/MathUtils.h"

namespace Ovis
{
	std::unique_ptr<Renderer::SceneData> Renderer::s_SceneData = std::make_unique<Renderer::SceneData>();

	void Renderer::BeginScene(const Camera& camera)
	{
		OV_RENDER_PROFILE_FUNC();

		s_SceneData->ProjectionMatrix = camera.GetProjectionMatrix();
		s_SceneData->ViewMatrix = camera.GetViewMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const Transform transform)
	{
		OV_RENDER_PROFILE_FUNC();

		glm::mat4 model(1.0f);
		model = glm::translate(model, transform.Position);
		model = glm::scale(model, transform.Scale);
		model = glm::rotate(model, glm::radians(transform.Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		shader->Bind();
		shader->SetUniform("model", model);
		shader->SetUniform("view", s_SceneData->ViewMatrix);
		shader->SetUniform("projection", s_SceneData->ProjectionMatrix);
		// shader->SetUniform("view", glm::mat4(1.0f));
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}