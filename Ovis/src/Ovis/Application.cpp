#include "ovpch.h"
#include "Application.h"

#include "Ovis/Events/ApplicationEvent.h"
#include "Ovis/Log.h"

namespace Ovis {
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	}

	Application::~Application() 
	{
	}

	void Application::OnEvent(Event& e) 
	{
		OV_CORE_INFO("{0}", e.ToString());
	}

	void Application::Run()
	{
		while (m_Running) {
			
			m_Window->OnUpdate();
		}
	}
}