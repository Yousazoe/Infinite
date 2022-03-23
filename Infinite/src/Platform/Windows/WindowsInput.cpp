#include "ifnpch.h"
#include "Platform/Windows/WindowsInput.h"
#include "Infinite/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Infinite {

	Scope<Input> Input::s_Instance = CreateScope<WindowsInput>();

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS
			    || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float WindowsInput::GetMouseXImpl()
	{
		 // auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		// double xpos, ypos;
		// glfwGetCursorPos(window, &xpos, &ypos);

		// return (float)xpos;

		auto [x, y] = GetMousePositionImpl();
		return (float)x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		// auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		// double xpos, ypos;
		// glfwGetCursorPos(window, &xpos, &ypos);

		// return (float)ypos;

		auto [x, y] = GetMousePositionImpl();
		return (float)y;
	}

}