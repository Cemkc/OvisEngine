#pragma once

#include "Core.h"
#include "Window.h"
#include "Ovis/LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Ovis/imgui/ImguiLayer.h"

#include "Ovis/Renderer/Shader.h"
#include "Ovis/Renderer/Buffer.h"

namespace Ovis {

	class Application
	{
	private:
		static Application* s_Instance;
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int m_VertexArray;
		unsigned int m_ShaderProgram;

		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
