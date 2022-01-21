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
		// IFN_INFO("ExampleLayer::Update");

		if (Infinite::Input::IsKeyPressed(IFN_KEY_TAB))
			IFN_INFO("Tab key is pressed (poll)!");
	}

	void OnEvent(Infinite::Event& event) override
	{
			if (event.GetEventType() == Infinite::EventType::KeyPressed)
			{
				Infinite::KeyPressedEvent& e = (Infinite::KeyPressedEvent&)event;
				if (Infinite::Input::IsKeyPressed(IFN_KEY_TAB))
					IFN_INFO("Tab key is pressed (event)!");

				IFN_TRACE("{0}", (char)e.GetKeyCode());
			} 
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