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
	m_RocketTexture = Ovis::Texture2D::Create("assets/textures/OneRocket.png");
	m_FrogTexture = Ovis::Texture2D::Create("assets/textures/Frog.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate()
{
	OV_PROFILE_FUNC();
	OV_TRACE("Delta Time: {0}s ({1}ms)", Ovis::Time::DeltaTime(), Ovis::Time::DeltaTime() * 1000);

	Ovis::Renderer2D::Instance().ResetStats();

	{
		OV_PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController->OnUpdate();
	}

	{
		OV_PROFILE_SCOPE("Renderer Prep");
		Ovis::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Ovis::RenderCommand::Clear();
	}

	{
		OV_PROFILE_SCOPE("Renderer Draw");
		Ovis::Renderer2D::Instance().BeginScene(m_CameraController->GetCamera());

		Ovis::GameEntity entity;

		entity.GetTransform() =
		{
			glm::vec3(0.0f, 0.0f, -0.1f),
			glm::vec3(0.0f, 0.0f, 45.0f),
			glm::vec3(5.0f, 5.0f, 1.0f)
		};

		Ovis::Renderer2D::Instance().SubmitQuad(entity, *m_CheckerBoardTexture.get(), 10.0f);

		float offset = 0.1f;

		for (int x = 0; x < 100; x++)
		{
			for (int y = 0; y < 100; y++)
			{
				Ovis::GameEntity entity;

				entity.GetTransform() =
				{
					glm::vec3(0.0f, 0.0f, -0.1f),
					glm::vec3(0.0f, 0.0f, 45.0f),
					glm::vec3(5.0f, 5.0f, 1.0f)
				};

				glm::vec4 color = (y + x) % 2 == 0 ? m_QuadColor1 : m_QuadColor2;

				Ovis::Renderer2D::Instance().SubmitQuad(entity, color);
			}
		}

		Ovis::Renderer2D::Instance().EndScene();
	}
}

//int frameRate = 0;
//float frameTime = 0;

void Sandbox2D::OnImGuiRender()
{
	//OV_PROFILE_FUNC();

	//ImGui::Begin("Settings");
	//ImGui::ColorEdit4("Quad Color 1", glm::value_ptr(m_QuadColor1));
	//ImGui::ColorEdit4("Quad Color 2", glm::value_ptr(m_QuadColor2));


	//ImGui::Text("Performance Metrics:");
	//if (Ovis::Time::GetTime() - m_ImGuiRefreshTimer > 1)
	//{
	//	frameRate = (int)(1 / Ovis::Time::DeltaTime());
	//	frameTime = Ovis::Time::DeltaTime() * 1000;
	//	m_ImGuiRefreshTimer = Ovis::Time::GetTime();
	//}

	//ImGui::Text("Frame Rate: %d fps", frameRate);
	//ImGui::Text("Frame Time: %f ms", frameTime);

	//auto stats = Ovis::Renderer2D::Instance().GetStats();
	//ImGui::Text("Renderer2D Stats:");
	//ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	//ImGui::Text("Quads: %d", stats.QuadCount);
	//ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	//ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	//ImGui::End();
}

void Sandbox2D::OnEvent(Ovis::Event& event)
{
	m_CameraController->OnEvent(event);
}
