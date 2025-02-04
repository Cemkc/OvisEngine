#pragma once
#include "Ovis.h"
#include "Ovis/Core/EntryPoint.h"

#include "Sandbox2D.h"
#include "Managers/GridManager.h"
#include "Managers/AnimationManager.h"

class Sandbox : public Ovis::Application
{
public:
	Sandbox()
	{
		// PushLayer(new Sandbox2D());
		PushLayer(new GridManager());
		PushLayer(new AnimationManager());
	}

	~Sandbox(){}
};

Ovis::Application* Ovis::CreateApplication()
{
	return new Sandbox();
}