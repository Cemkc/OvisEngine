#include "ovpch.h"
#include "Application.h"

#include "Ovis/Core/Time.h"
#include "Ovis/Core/Log.h"
#include "Ovis/Core/Input.h"

#include "Ovis/Renderer/Renderer.h"
#include "Ovis/Renderer/Renderer2D.h"
#include "Ovis/Renderer/SnapRenderer2D.h"
#include "Ovis/Renderer/BatchRenderer2D.h"

namespace Ovis {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		OV_PROFILE_FUNC();

		OV_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		{
			OV_PROFILE_SCOPE("Application::Application - Window Creation");

			m_Window = std::unique_ptr<Window>(Window::Create());
			m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
		}

		{
			OV_PROFILE_SCOPE("Application::Application - Renderer Init");

			Renderer2D* renderer = new BatchRenderer2D();

			RenderCommand::Init();
			Renderer2D::Instance().Init();
		}

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_Time = new Time();
	}

	Application::~Application() 
	{
		OV_PROFILE_FUNC();
	}

	void Application::Run()
	{
		OV_PROFILE_FUNC();

		while (m_Running) 
		{
			for each(auto callback in m_EventCallbacks)
			{
				AppRenderEvent e(LoopState::Begin);
				callback(e);
			}

			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();

			for each(auto callback in m_EventCallbacks)
			{
				AppRenderEvent e(LoopState::End);
				callback(e);
			}
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));

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

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		RenderCommand::OnWindowResize(m_Window->GetWidth(), m_Window->GetHeight());
		return false;
	}

}