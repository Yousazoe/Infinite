#pragma once

#ifdef IFN_PLATFORM_WINDOWS

extern Infinite::Application* Infinite::CreateApplication();

int main(int argc,char** argv) {
	printf("Infinite Engine");
	auto app = Infinite::CreateApplication();
	app->Run();
	delete app;
}

#endif