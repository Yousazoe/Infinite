#pragma once

#ifdef IFN_PLATFORM_WINDOWS

extern Infinite::Application* Infinite::CreateApplication();

int main(int argc,char** argv) {
	Infinite::Log::Init();
	IFN_CORE_WARN("Initialized Log!");
	int a = 5;
	IFN_INFO("Hello! {0}",a);

	printf("Infinite Engine");
	auto app = Infinite::CreateApplication();
	app->Run();
	delete app;
}

#endif