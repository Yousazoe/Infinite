#include "ifnpch.h"
#include "Infinite/Core/Window.h"

#ifdef IFN_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Infinite {
	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef IFN_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		IFN_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}