#pragma once

#ifdef OV_PLATFORM_WINDOWS

extern Ovis::Application* Ovis::CreateApplication();

void main(int argc, char** argv) 
{
	int a = 5;
	Ovis::Log::Init();
	OV_CORE_ERROR("Initialized Core Log!");
	OV_WARN("Hello Var={0}", a);

	auto app = Ovis::CreateApplication();
	app->Run();
	delete app;
}

#endif
