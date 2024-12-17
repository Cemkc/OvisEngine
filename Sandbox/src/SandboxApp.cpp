#pragma once
#include <Ovis.h>

#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Ovis::Layer
{
private:
	std::shared_ptr<Ovis::Shader> m_Shader;
	std::shared_ptr<Ovis::VertexArray> m_VertexArray;

	std::shared_ptr<Ovis::Shader> m_FlatShader;
	std::shared_ptr<Ovis::VertexArray> m_SquareVertexArray;

	std::shared_ptr<Ovis::Camera> m_Camera;

	float m_CamMoveSpeed = 3.0f;

	float m_CamRotation = 0.0f;
	float m_CamRotationSpeed = 30.0f;

	float m_SquareWidht = 0.0f;
	glm::vec4 m_SquareColor = glm::vec4(1.0f);
public:
	ExampleLayer()
		: Layer("Example")
	{
		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f,  0.8f, 0.8f, 0.1f, 1.0f,
			 0.5f, -0.5f, 0.0f,  0.8f, 0.1f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f,	 0.1f, 0.8f, 0.8f,	1.0f,
		};

		std::shared_ptr<Ovis::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Ovis::VertexBuffer::Create(vertices, sizeof(vertices)));

		float squareVertices[] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
		};

		std::shared_ptr<Ovis::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(Ovis::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		Ovis::BufferLayout layout
		{
			{ Ovis::ShaderDataType::Float3, "aPos" },
			{ Ovis::ShaderDataType::Float4, "aColor" }
		};

		vertexBuffer->SetLayout(layout);

		uint32_t indices[] =
		{
			0, 1, 2
		};

		std::shared_ptr<Ovis::IndexBuffer> indexBuffer;
		indexBuffer.reset(Ovis::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		Ovis::BufferLayout squareLayout
		{
			{ Ovis::ShaderDataType::Float3, "aPos" },
		};

		squareVertexBuffer->SetLayout(squareLayout);

		uint32_t squareIndices[] =
		{
			0, 1, 2, 2, 3, 0
		};

		std::shared_ptr<Ovis::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(Ovis::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));


		m_VertexArray.reset(Ovis::VertexArray::Create());
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVertexArray.reset(Ovis::VertexArray::Create());
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		const std::string vertexSrc = R"(
			#version 330 core
			layout (location = 0) in vec3 aPos;
			layout (location = 1) in vec4 aColor;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;

			out vec4 vColor;

			void main(){
				gl_Position = projection * view * model * vec4(aPos, 1.0);
				vColor = aColor;
			}
		)";

		const std::string fragmentSrc = R"(
			#version 330 core

			in vec4 vColor;

			out vec4 FragColor;

			void main(){
				FragColor = vColor;
			}
		)";

		const std::string flatShaderVsrc = R"(
			#version 330 core
			layout (location = 0) in vec3 aPos;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;

			void main(){
				gl_Position = projection * view * model * vec4(aPos, 1.0);
			}
		)";

		const std::string flatShaderFsrc = R"(
			#version 330 core

			out vec4 FragColor;

			uniform vec4 u_Color;

			void main(){
				FragColor = u_Color;
			}
		)";

		m_Shader.reset(Ovis::Shader::Create(vertexSrc, fragmentSrc));

		m_FlatShader.reset(Ovis::Shader::Create(flatShaderVsrc, flatShaderFsrc));

		OV_CORE_TRACE("{0}", Ovis::Application::Get().GetWindow().GetWidth());
		OV_CORE_TRACE("{0}", Ovis::Application::Get().GetWindow().GetHeight());
		OV_CORE_TRACE("{0}", Ovis::Application::Get().GetWindow().AspectRatio());

		/*m_Camera.reset(new Ovis::PerspectiveCamera(45.0f, Ovis::Application::Get().GetWindow().AspectRatio(), 0.1f, 100.0f));*/
		m_Camera.reset(new Ovis::OrthographicCamera(-1.0f, 1.0f, -1.0f, 1.0f));
		m_Camera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
		std::static_pointer_cast<Ovis::OrthographicCamera>(m_Camera)->SetRotation(45.0f);

		m_SquareWidht = 0.1f;
	}

	void OnUpdate() override
	{
		OV_TRACE("Delta Time: {0}s ({1}ms)", Ovis::Time::DeltaTime(), Ovis::Time::DeltaTime() * 1000);

		Ovis::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Ovis::RenderCommand::Clear();

		Ovis::Renderer::BeginScene(m_Camera);

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

		// Ovis::Renderer::Submit(m_VertexArray, m_Shader, transform);

		Ovis::Renderer::EndScene();

		glm::vec3 camPosition = m_Camera->GetPosition();

		if (Ovis::Input::IsKeyPressed(OV_KEY_W))
		{
			camPosition.y += 1.0f * m_CamMoveSpeed * Ovis::Time::DeltaTime();
		}
		if (Ovis::Input::IsKeyPressed(OV_KEY_S))
		{
			camPosition.y -= 1.0f * m_CamMoveSpeed * Ovis::Time::DeltaTime();
		}
		if (Ovis::Input::IsKeyPressed(OV_KEY_A))
		{
			camPosition.x -= 1.0f * m_CamMoveSpeed * Ovis::Time::DeltaTime();
		}
		if (Ovis::Input::IsKeyPressed(OV_KEY_D))
		{
			camPosition.x += 1.0f * m_CamMoveSpeed * Ovis::Time::DeltaTime();
		}

		if (Ovis::Input::IsKeyPressed(OV_KEY_E))
		{
			m_CamRotation -= m_CamRotationSpeed * Ovis::Time::DeltaTime();
		}
		if (Ovis::Input::IsKeyPressed(OV_KEY_Q))
		{
			m_CamRotation += m_CamRotationSpeed * Ovis::Time::DeltaTime();
		}

		m_Camera->SetPosition(camPosition);
		auto camera = std::static_pointer_cast<Ovis::OrthographicCamera>(m_Camera);
		camera->SetRotation(m_CamRotation);

	}

	void OnEvent(Ovis::Event& event) override
	{
		// OV_TRACE("{0}", event.ToString());
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