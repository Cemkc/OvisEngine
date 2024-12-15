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