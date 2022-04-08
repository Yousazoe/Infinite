#pragma once
#include "Infinite/Core/Core.h"

#ifdef IFN_PLATFORM_WINDOWS

extern Infinite::Application* Infinite::CreateApplication();

int main(int argc,char** argv) {
	Infinite::Log::Init();

	IFN_PROFILE_BEGIN_SESSION("Startup", "InfiniteProfiles-Startup.json");
	auto app = Infinite::CreateApplication();
	IFN_PROFILE_END_SESSION();

	IFN_PROFILE_BEGIN_SESSION("Runtime", "Infinite-Runtime.json");
	app->Run();
	IFN_PROFILE_END_SESSION();

	IFN_PROFILE_BEGIN_SESSION("Startup", "InfiniteProfile-Shutdown.json");
	delete app;
	IFN_PROFILE_END_SESSION();
}

#endif