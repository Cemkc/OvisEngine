#pragma once
#include <Ovis.h>

#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Ovis::Layer
{
private:
	std::shared_ptr<Ovis::VertexArray> m_SquareVertexArray;

	Ovis::ShaderLibrary m_ShaderLibrary;
	std::shared_ptr<Ovis::Shader> m_FlatShader;

	std::shared_ptr<Ovis::Texture2D> m_CheckerBoardTexture;
	std::shared_ptr<Ovis::Texture2D> m_LogoTexture;

	std::shared_ptr<Ovis::OrthographicCameraController> m_CameraController;

	float m_SquareWidht = 0.0f;
	glm::vec4 m_SquareColor = glm::vec4(1.0f);
public:
	ExampleLayer()
		: Layer("Example")
	{
		float aspectRatio = Ovis::Application::Get().GetWindow().AspectRatio();
		m_CameraController = std::make_shared<Ovis::OrthographicCameraController>(aspectRatio, true);

		float squareVertices[] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,	1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,	1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,	0.0f, 1.0f
		};

		std::shared_ptr<Ovis::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(Ovis::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		Ovis::BufferLayout squareLayout
		{
			{ Ovis::ShaderDataType::Float3, "aPos" },
			{ Ovis::ShaderDataType::Float2, "a_TexCoord" }
		};

		squareVertexBuffer->SetLayout(squareLayout);

		uint32_t squareIndices[] =
		{
			0, 1, 2, 2, 3, 0
		};

		std::shared_ptr<Ovis::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(Ovis::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

		m_SquareVertexArray.reset(Ovis::VertexArray::Create());
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		const std::string vertexSrc = R"(
			#version 330 core
			layout (location = 0) in vec3 aPos;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;

			void main(){
				gl_Position = projection * view * model * vec4(aPos, 1.0);
			}
		)";

		const std::string fragmentSrc = R"(
			#version 330 core
			out vec4 FragColor;

			uniform vec4 u_Color;

			void main(){
				FragColor = u_Color;
			}
		)";

		m_FlatShader = Ovis::Shader::Create("Flat", vertexSrc, fragmentSrc);

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		OV_CORE_TRACE("{0}", Ovis::Application::Get().GetWindow().GetWidth());
		OV_CORE_TRACE("{0}", Ovis::Application::Get().GetWindow().GetHeight());
		OV_CORE_TRACE("{0}", Ovis::Application::Get().GetWindow().AspectRatio());

		m_CheckerBoardTexture = Ovis::Texture2D::Create("assets/textures/Checkerboard.png");

		m_LogoTexture = Ovis::Texture2D::Create("assets/textures/anka.png");

		textureShader->Bind();
		textureShader->SetUniform("u_Texture", 0);

		m_SquareWidht = 0.1f;
	}

	void OnUpdate() override
	{
		OV_TRACE("Delta Time: {0}s ({1}ms)", Ovis::Time::DeltaTime(), Ovis::Time::DeltaTime() * 1000);
		m_CameraController->OnUpdate();

		Ovis::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Ovis::RenderCommand::Clear();

		Ovis::Renderer::BeginScene(m_CameraController->GetCamera());

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				Ovis::Transform transform =
				{
					glm::vec3(x * 0.11f, y * 0.11f, 0.0f),
					glm::vec3(0.0f, 0.0f, 0.0f),
					glm::vec3(0.1f, 0.1f, 0.1f)
				};

				m_FlatShader->SetUniform("u_Color", m_SquareColor);
				Ovis::Renderer::Submit(m_SquareVertexArray, m_FlatShader, transform);
			}
		}

		Ovis::Transform transform =
		{
			glm::vec3(0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(1.5f, 1.5f, 1.5f)
		};

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_CheckerBoardTexture->Bind();
		Ovis::Renderer::Submit(m_SquareVertexArray, textureShader, transform);

		m_LogoTexture->Bind();
		Ovis::Renderer::Submit(m_SquareVertexArray, textureShader, transform);

		Ovis::Renderer::EndScene();
	}

	void OnEvent(Ovis::Event& event) override
	{
		// OV_TRACE("{0}", event.ToString());
		m_CameraController->OnEvent(event);
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}
};

class Sandbox : public Ovis::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox(){}
};

Ovis::Application* Ovis::CreateApplication()
{
	return new Sandbox();
}