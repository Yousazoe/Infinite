#include <Infinite.h>

class Sandbox : public Infinite::Application {
public:
	Sandbox() {}
	~Sandbox() {}
};

void main() {
	Sandbox* sandbox = new Sandbox();
	sandbox->Run();
	delete sandbox;
}