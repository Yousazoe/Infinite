#include <Infinite.h>

class Sandbox : public Infinite::Application {
public:
	Sandbox() {}
	~Sandbox() {}
};

Infinite::Application* Infinite::CreateApplication() {
	return new Sandbox();
}