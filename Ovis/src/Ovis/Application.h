#pragma once

#include "Core.h"
#include "Window.h"
#include "Ovis/Time.h"
#include "Ovis/LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Ovis/imgui/ImguiLayer.h"

#include "Ovis/Renderer/Shader.h"
#include "Ovis/Renderer/Buffer.h"
#include "Ovis/Renderer/VertexArray.h"
#include "Ovis/Renderer/Camera.h"

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
		inline static int DeltaTime();

		void AddEventCallback(const std::function<void(Event&)>& callback) { m_EventCallbacks.push_back(callback); }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
