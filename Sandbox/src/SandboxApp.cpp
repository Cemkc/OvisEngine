#include <Ovis.h>

class Sandbox : public Ovis::Application
{
public:
	Sandbox(){}
	~Sandbox(){}
};

Ovis::Application* Ovis::CreateApplication()
{
	return new Sandbox();
}