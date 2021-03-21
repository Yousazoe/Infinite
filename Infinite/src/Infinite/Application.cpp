#include "ifnpch.h"
#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "Log.h"

namespace Infinite {
	Application::Application() {}
	Application::~Application() {}

	void Application::Run() {

		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			IFN_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			IFN_TRACE(e);
		}


		while (true);
	}
}
