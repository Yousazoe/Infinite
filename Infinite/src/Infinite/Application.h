#pragma once

#include "Core.h"

namespace Infinite {
	class INFINITE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	//To be define in CLIENT
	Application* CreateApplication();
}