#pragma once
#include "Ovis.h"
#include "Ovis/Core/EntryPoint.h"

#include "Sandbox2D.h"
#include "ExampleLayer.h"

class Sandbox : public Ovis::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		// PushLayer(new Sandbox2D());
	}

	~Sandbox(){}
};

Ovis::Application* Ovis::CreateApplication()
{
	return new Sandbox();
}