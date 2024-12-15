#include "ovpch.h"
#include "Application.h"

#include "Ovis/Time.h"
#include "Ovis/Log.h"
#include "Ovis/Input.h"

#include "Ovis/Renderer/Renderer.h"

namespace Ovis {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		OV_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_Time = new Time();

		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f,  0.8f, 0.8f, 0.1f, 1.0f,
			 0.5f, -0.5f, 0.0f,  0.8f, 0.1f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f,	 0.1f, 0.8f, 0.8f,	1.0f,
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		float squareVertices[] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
		};

		std::shared_ptr<VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		BufferLayout layout
		{
			{ ShaderDataType::Float3, "aPos" },
			{ ShaderDataType::Float4, "aColor" }
		};

		vertexBuffer->SetLayout(layout);

		uint32_t indices[] =
		{
			0, 1, 2
		};

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		BufferLayout squareLayout
		{
			{ ShaderDataType::Float3, "aPos" },
		};

		squareVertexBuffer->SetLayout(squareLayout);

		uint32_t squareIndices[] =
		{
			0, 1, 2, 2, 3, 0
		};

		std::shared_ptr<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		

		m_VertexArray.reset(VertexArray::Create());
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVertexArray.reset(VertexArray::Create());
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

		m_Shader.reset(Shader::Create(vertexSrc, fragmentSrc));

		m_BlueShader.reset(Shader::Create(blueShaderVsrc, blueShaderFsrc));

		OV_CORE_TRACE("{0}", Application::Get().GetWindow().GetWidth());
		OV_CORE_TRACE("{0}", Application::Get().GetWindow().GetHeight());
		OV_CORE_TRACE("{0}", Application::Get().GetWindow().AspectRatio());
		m_Camera.reset(new PerspectiveCamera(45.0f, Application::Get().GetWindow().AspectRatio(), 0.1f, 100.0f));
		m_Camera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
	}

	Application::~Application() 
	{
	}

	void Application::Run()
	{
		while (m_Running) 
		{
			for each(auto callback in m_EventCallbacks)
			{
				AppRenderEvent e(LoopState::Begin);
				callback(e);
			}

			RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
			RenderCommand::Clear();

			Renderer::BeginScene(m_Camera);

			Renderer::Submit(m_SquareVertexArray, m_BlueShader);

			Renderer::Submit(m_VertexArray, m_Shader);

			Renderer::EndScene();

			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();

			//OV_CORE_TRACE("Delta Time: {0}", Time::GetDeltaTime());
			//OV_CORE_TRACE("Time: {0}", Time::GetTime());

			for each(auto callback in m_EventCallbacks)
			{
				AppRenderEvent e(LoopState::End);
				callback(e);
			}

			//glm::vec3 camPosition = m_Camera->GetPosition();
			//float camSpeed = 0.3f;
			//if (Input::IsKeyPressed(OV_KEY_W))
			//{
			//	camPosition.z -= 1.0f * camSpeed * Application::DeltaTime();
			//}
			//if (Input::IsKeyPressed(OV_KEY_S))
			//{
			//	camPosition.z += 1.0f * camSpeed * Application::DeltaTime();
			//}
			//if (Input::IsKeyPressed(OV_KEY_A))
			//{
			//	camPosition.x -= 1.0f * camSpeed * Application::DeltaTime();
			//}
			//if (Input::IsKeyPressed(OV_KEY_D))
			//{
			//	camPosition.x += 1.0f * camSpeed * Application::DeltaTime();
			//}

			//m_Camera->SetPosition(camPosition);
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
		// OV_CORE_INFO("{0}", e.ToString());

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled) {
				break;
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	inline int Application::DeltaTime()
	{
		return s_Instance->Get().GetWindow().GetTime();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return false;
	}



}