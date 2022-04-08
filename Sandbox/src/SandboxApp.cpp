#include <Infinite.h>
#include <Infinite/Core/EntryPoint.h>

#include "Sandbox2D.h"

#include "ExampleLayer.h"

class Sandbox : public Infinite::Application {
public:
	Sandbox() 
	{
		// PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}
	~Sandbox() {}
};

Infinite::Application* Infinite::CreateApplication() {
	return new Sandbox();
}