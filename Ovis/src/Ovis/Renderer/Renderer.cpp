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

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader)
	{
		shader->Bind();
		shader->SetUniform("model", glm::mat4(1.0f));
		shader->SetUniform("view", m_View);
		shader->SetUniform("projection", m_Projection);
		//shader->SetUniform("view", glm::mat4(1.0f));
		//shader->SetUniform("projection", glm::mat4(1.0f));
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}