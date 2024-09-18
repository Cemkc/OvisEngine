#include "Application.h"

#include "Ovis/Events/ApplicationEvent.h"
#include "Ovis/Log.h"

namespace Ovis {
	Application::Application()
	{
	}

	Application::~Application() 
	{
	}

	void Application::Run()
	{

		WindowResizeEvent e(1280, 720);
		OV_TRACE(e.ToString());

		while(true);
	}
}