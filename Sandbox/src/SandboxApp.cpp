#pragma once
#include <Ovis.h>

class ExampleLayer : public Ovis::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}
	void OnUpdate() override
	{
		// OV_INFO("ExampleLayer::Update");
	}
	void OnEvent(Ovis::Event& event) override
	{
		OV_TRACE("{0}", event.ToString());
	}
};

class Sandbox : public Ovis::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Ovis::ImGuiLayer());
	}

	~Sandbox(){}
};

Ovis::Application* Ovis::CreateApplication()
{
	return new Sandbox();
}