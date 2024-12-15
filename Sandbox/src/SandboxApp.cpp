#pragma once
#include <Ovis.h>

class ExampleLayer : public Ovis::Layer
{
private:
	std::shared_ptr<Ovis::Shader> m_Shader;
	std::shared_ptr<Ovis::VertexArray> m_VertexArray;

	std::shared_ptr<Ovis::Shader> m_BlueShader;
	std::shared_ptr<Ovis::VertexArray> m_SquareVertexArray;

	std::shared_ptr<Ovis::Camera> m_Camera;
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

		const std::string blueShaderVsrc = R"(
			#version 330 core
			layout (location = 0) in vec3 aPos;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;

			void main(){
				gl_Position = projection * view * model * vec4(aPos, 1.0);
			}
		)";

		const std::string blueShaderFsrc = R"(
			#version 330 core

			out vec4 FragColor;

			void main(){
				FragColor = vec4(0.1, 0.2, 0.8, 1.0);
			}
		)";

		m_Shader.reset(Ovis::Shader::Create(vertexSrc, fragmentSrc));

		m_BlueShader.reset(Ovis::Shader::Create(blueShaderVsrc, blueShaderFsrc));

		OV_CORE_TRACE("{0}", Ovis::Application::Get().GetWindow().GetWidth());
		OV_CORE_TRACE("{0}", Ovis::Application::Get().GetWindow().GetHeight());
		OV_CORE_TRACE("{0}", Ovis::Application::Get().GetWindow().AspectRatio());

		m_Camera.reset(new Ovis::PerspectiveCamera(45.0f, Ovis::Application::Get().GetWindow().AspectRatio(), 0.1f, 100.0f));
		m_Camera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
	}

	void OnUpdate() override
	{
		Ovis::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Ovis::RenderCommand::Clear();

		Ovis::Renderer::BeginScene(m_Camera);

		Ovis::Renderer::Submit(m_SquareVertexArray, m_BlueShader);

		Ovis::Renderer::Submit(m_VertexArray, m_Shader);

		Ovis::Renderer::EndScene();
	}

	void OnEvent(Ovis::Event& event) override
	{
		// OV_TRACE("{0}", event.ToString());
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