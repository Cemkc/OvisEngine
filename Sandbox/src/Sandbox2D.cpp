#include "ovpch.h"

#include "Sandbox2D.h"

#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D")
{
}

void Sandbox2D::OnAttach()
{
	float aspectRatio = Ovis::Application::Get().GetWindow().AspectRatio();
	m_CameraController = std::make_shared<Ovis::OrthographicCameraController>(aspectRatio, true);

	m_CheckerBoardTexture = Ovis::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate()
{
	OV_TRACE("Delta Time: {0}s ({1}ms)", Ovis::Time::DeltaTime(), Ovis::Time::DeltaTime() * 1000);
	m_CameraController->OnUpdate();

	Ovis::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Ovis::RenderCommand::Clear();

	Ovis::Renderer2D::BeginScene(m_CameraController->GetCamera());

	Ovis::Transform transform =
	{
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 2.0f, 1.0f)
	};

	Ovis::Renderer2D::DrawQuad(transform, m_SquareColor);

	transform =
	{
		glm::vec3(0.0f, 0.0f, -0.1f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(10.0f, 10.0f, 1.0f)
	};

	Ovis::Renderer2D::DrawQuad(transform, *m_CheckerBoardTexture);

	Ovis::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Ovis::Event& event)
{
	m_CameraController->OnEvent(event);
}
