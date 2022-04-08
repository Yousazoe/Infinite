#pragma once

namespace Infinite
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define IFN_MOUSE_BUTTON_0      ::Infinite::Mouse::Button0
#define IFN_MOUSE_BUTTON_1      ::Infinite::Mouse::Button1
#define IFN_MOUSE_BUTTON_2      ::Infinite::Mouse::Button2
#define IFN_MOUSE_BUTTON_3      ::Infinite::Mouse::Button3
#define IFN_MOUSE_BUTTON_4      ::Infinite::Mouse::Button4
#define IFN_MOUSE_BUTTON_5      ::Infinite::Mouse::Button5
#define IFN_MOUSE_BUTTON_6      ::Infinite::Mouse::Button6
#define IFN_MOUSE_BUTTON_7      ::Infinite::Mouse::Button7
#define IFN_MOUSE_BUTTON_LAST   ::Infinite::Mouse::ButtonLast
#define IFN_MOUSE_BUTTON_LEFT   ::Infinite::Mouse::ButtonLeft
#define IFN_MOUSE_BUTTON_RIGHT  ::Infinite::Mouse::ButtonRight
#define IFN_MOUSE_BUTTON_MIDDLE ::Infinite::Mouse::ButtonMiddle