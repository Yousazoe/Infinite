#include <Infinite.h>

class ExampleLayer : public Infinite::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{

	}

	void OnUpdate() override
	{
		IFN_INFO("ExampleLayer::Update");
	}

	void OnEvent(Infinite::Event& event) override
	{
		IFN_TRACE("{0}",event);
	}

};

class Sandbox : public Infinite::Application {
public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Infinite::ImGuiLayer());
	}
	~Sandbox() {}
};

Infinite::Application* Infinite::CreateApplication() {
	return new Sandbox();
}