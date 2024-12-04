#include "ovpch.h"
#include "Application.h"

#include "Ovis/Log.h"
#include "Ovis/Input.h"

#include "glad/glad.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Ovis {

	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
				case Ovis::ShaderDataType::Float:    return GL_FLOAT;
				case Ovis::ShaderDataType::Float2:   return GL_FLOAT;
				case Ovis::ShaderDataType::Float3:   return GL_FLOAT;
				case Ovis::ShaderDataType::Float4:   return GL_FLOAT;
				case Ovis::ShaderDataType::Mat3:     return GL_FLOAT;
				case Ovis::ShaderDataType::Mat4:     return GL_FLOAT;
				case Ovis::ShaderDataType::Int:      return GL_INT;
				case Ovis::ShaderDataType::Int2:     return GL_INT;
				case Ovis::ShaderDataType::Int3:     return GL_INT;
				case Ovis::ShaderDataType::Int4:     return GL_INT;
				case Ovis::ShaderDataType::Bool:     return GL_BOOL;
			}

			OV_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	Application::Application()
	{
		OV_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f,  0.8f, 0.8f, 0.1f, 1.0f,
			 0.5f, -0.5f, 0.0f,  0.8f, 0.1f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f,	 0.1f, 0.8f, 0.8f,	1.0f,
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		uint32_t indices[] =
		{
			0, 1, 2
		};

		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		{
			BufferLayout layout
			{
				{ ShaderDataType::Float3, "aPos" },
				{ ShaderDataType::Float4, "aColor" }
			};

			m_VertexBuffer->SetLayout(layout);
		}

		const auto& bufferLayout = m_VertexBuffer->GetLayout();
		int index = 0;
		for (const auto& attribute : bufferLayout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, attribute.GetComponentCount(), 
				ShaderDataTypeToOpenGLBaseType(attribute.Type), 
				attribute.Normalized ? GL_TRUE : GL_FALSE,
				bufferLayout.GetSrtide(),
				(void*)attribute.Offset);

			index++;
		}

		const std::string vertexSrc = R"(
			#version 330 core
			layout (location = 0) in vec3 aPos;
			layout (location = 1) in vec4 aColor;

			out vec4 vColor;

			void main(){
				gl_Position = vec4(aPos, 1.0);
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

		m_Shader.reset(Shader::Create(vertexSrc, fragmentSrc));
	}

	Application::~Application() 
	{
	}

	void Application::Run()
	{
		while (m_Running) 
		{
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glBindVertexArray(m_VertexArray);
			m_Shader->Bind();
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
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

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return false;
	}



}