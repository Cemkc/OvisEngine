#pragma once

#ifdef OV_PLATFORM_WINDOWS

extern Ovis::Application* Ovis::CreateApplication();

void main(int argc, char** argv) 
{
	std::cout << "Ovis Engine\n";
	auto app = Ovis::CreateApplication();
	app->Run();
	delete app;
}

#endif
