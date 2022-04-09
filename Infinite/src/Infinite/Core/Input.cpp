#include "ifnpch.h"
#include "Infinite/Core/Input.h"

#ifdef IFN_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsInput.h"
#endif

namespace Infinite {

	Scope<Input> Input::s_Instance = Input::Create();

	Scope<Input> Input::Create()
	{
#ifdef IFN_PLATFORM_WINDOWS
		return CreateScope<WindowsInput>();
#else
		IFN_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}
}