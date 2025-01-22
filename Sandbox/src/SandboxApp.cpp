#pragma once
#include "Ovis.h"
#include "Ovis/Core/EntryPoint.h"

#include "Sandbox2D.h"
#include "Managers/GridManager.h"

class Sandbox : public Ovis::Application
{
public:
	Sandbox()
	{
		// PushLayer(new Sandbox2D());
		PushLayer(new GridManager());
	}

	~Sandbox(){}
};

Ovis::Application* Ovis::CreateApplication()
{
	return new Sandbox();
}