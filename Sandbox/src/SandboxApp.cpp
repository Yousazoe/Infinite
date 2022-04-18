#include <Infinite.h>
#include <Infinite/Core/EntryPoint.h>

#include "Sandbox2D.h"

#include "ExampleLayer.h"

#include "Example/Icarus/IcarusGameLayer.h"

class Sandbox : public Infinite::Application {
public:
	Sandbox() 
	{
		// PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
		// PushLayer(new IcarusGameLayer());
	}
	~Sandbox() {}
};

Infinite::Application* Infinite::CreateApplication() {
	return new Sandbox();
}