#include "ovpch.h"
#include "Renderer.h"

#include <glad/glad.h>
#include "Ovis/MathUtils.h"

namespace Ovis
{
	glm::mat4 Renderer::m_View;
	glm::mat4 Renderer::m_Projection;

	void Renderer::BeginScene(const std::shared_ptr<Camera> camera)
	{
		m_View = camera->GetViewMatrix();
		m_Projection = camera->GetProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const Transform transform)
	{
		glm::mat4 model(1.0f);
		model = glm::translate(model, transform.Position);
		model = glm::scale(model, transform.Scale);
		model = glm::rotate(model, glm::radians(transform.Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		shader->Bind();
		shader->SetUniform("model", model);
		shader->SetUniform("view", m_View);
		shader->SetUniform("projection", m_Projection);
		//shader->SetUniform("view", glm::mat4(1.0f));
		//shader->SetUniform("projection", glm::mat4(1.0f));
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}