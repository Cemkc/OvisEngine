#pragma once

#include "Core.h"
#include "Window.h"
#include "Ovis/Core/Time.h"
#include "Ovis/Core/LayerStack.h"
#include "Ovis/Events/Event.h"
#include "Ovis/Events/ApplicationEvent.h"

#include "Ovis/imgui/ImguiLayer.h"

namespace Ovis {
	class Application
	{
	private:
		static Application* s_Instance;
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		Time* m_Time;

		std::vector<std::function<void(Event&)>> m_EventCallbacks;

	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

		void AddEventCallback(const std::function<void(Event&)>& callback) { m_EventCallbacks.push_back(callback); }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool Application::OnWindowResize(WindowResizeEvent& e);
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
