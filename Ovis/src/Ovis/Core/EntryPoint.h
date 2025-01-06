#pragma once

#ifdef OV_PLATFORM_WINDOWS

extern Ovis::Application* Ovis::CreateApplication();

void main(int argc, char** argv) 
{
	Ovis::Log::Init();

	OV_PROFILE_BEGIN_SESSION("Startup", "SandboxProfile-Startup.json");
	auto app = Ovis::CreateApplication();
	OV_PROFILE_END_SESSION();

	OV_PROFILE_BEGIN_SESSION("Runtime", "SandboxProfile-Runtime.json");
	app->Run();
	OV_PROFILE_END_SESSION();

	OV_PROFILE_BEGIN_SESSION("Shutdown", "SandboxProfile-Shutdown.json");
	delete app;
	OV_PROFILE_END_SESSION();
}

#endif
