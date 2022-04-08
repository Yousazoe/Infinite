#pragma once

namespace Infinite
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space			= 32,
		Apostrophe = 39,				/* ' */
		Comma		= 44,			/* , */
		Minus			= 45,			/* - */
		Period			= 46,			/* . */
		Slash			= 47,			/* / */

		D0				= 48,			/* 0 */
		D1				= 49,			/* 1 */
		D2				= 50,			/* 2 */
		D3				= 51,			/* 3 */
		D4				= 52,			/* 4 */
		D5				= 53,			/* 5 */
		D6				= 54,			/* 6 */
		D7				= 55,			/* 7 */
		D8				= 56,			/* 8 */
		D9				= 57,			/* 9 */

		Semicolon	= 59,			/* ; */
		Equal			= 61,			/* = */

		A					= 65,
		B					= 66,
		C					= 67,
		D					= 68,
		E					= 69,
		F					= 70,
		G					= 71,
		H					= 72,
		I					= 73,
		J					= 74,
		K					= 75,
		L					= 76,
		M					= 77,
		N					= 78,
		O					= 79,
		P					= 80,
		Q					= 81,
		R					= 82,
		S					= 83,
		T					= 84,
		U					= 85,
		V					= 86,
		W					= 87,
		X					= 88,
		Y					= 89,
		Z					= 90,

		LeftBracket		= 91,			/* [ */
		Backslash			= 92,			/* \ */
		RightBracket	= 93,			/* ] */
		GraveAccent	= 96,			/* ` */

		World1				= 161,			/* non-US #1 */
		World2				= 162,			/* non-US #2 */

		/* Function keys */
		Escape				= 256,
		Enter				= 257,
		Tab					= 258,
		Backspace		= 259,
		Insert				= 260,
		Delete				= 261,
		Right				= 262,
		Left					= 263,
		Down				= 264,
		Up					= 265,
		PageUp			= 266,
		PageDown		= 267,
		Home				= 268,
		End					= 269,
		CapsLock			= 280,
		ScrollLock		= 281,
		NumLock			= 282,
		PrintScreen		= 283,
		Pause				= 284,
		F1						= 290,
		F2						= 291,
		F3						= 292,
		F4						= 293,
		F5						= 294,
		F6						= 295,
		F7						= 296,
		F8						= 297,
		F9						= 298,
		F10					= 299,
		F11					= 300,
		F12					= 301,
		F13					= 302,
		F14					= 303,
		F15					= 304,
		F16					= 305,
		F17					= 306,
		F18					= 307,
		F19					= 308,
		F20					= 309,
		F21					= 310,
		F22					= 311,
		F23					= 312,
		F24					= 313,
		F25					= 314,

		/* Keypad */
		KP0					= 320,
		KP1					= 321,
		KP2					= 322,
		KP3					= 323,
		KP4					= 324,
		KP5					= 325,
		KP6					= 326,
		KP7					= 327,
		KP8					= 328,
		KP9					= 329,
		KPDecimal		= 330,
		KPDivide			= 331,
		KPMultiply		= 332,
		KPSubtract		= 333,
		KPAdd				= 334,
		KPEnter			= 335,
		KPEqual			= 336,

		LeftShift			= 340,
		LeftControl		= 341,
		LeftAlt				= 342,
		LeftSuper			= 343,
		RightShift			= 344,
		RightControl	= 345,
		RightAlt			= 346,
		RightSuper		= 347,
		Menu				= 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	};


}

// From glfw3.h
#define IFN_KEY_SPACE					::Infinite::Key::Space
#define IFN_KEY_APOSTROPHE      ::Infinite::Key::Apostrophe    /* ' */
#define IFN_KEY_COMMA				::Infinite::Key::Comma			 /* , */
#define IFN_KEY_MINUS					::Infinite::Key::Minus				/* - */
#define IFN_KEY_PERIOD					::Infinite::Key::Period				/* . */
#define IFN_KEY_SLASH					::Infinite::Key::Slash				/* / */
#define IFN_KEY_0							::Infinite::Key::D0
#define IFN_KEY_1							::Infinite::Key::D1
#define IFN_KEY_2							::Infinite::Key::D2
#define IFN_KEY_3							::Infinite::Key::D3
#define IFN_KEY_4							::Infinite::Key::D4
#define IFN_KEY_5							::Infinite::Key::D5
#define IFN_KEY_6							::Infinite::Key::D6
#define IFN_KEY_7							::Infinite::Key::D7
#define IFN_KEY_8							::Infinite::Key::D8
#define IFN_KEY_9							::Infinite::Key::D9
#define IFN_KEY_SEMICOLON			::Infinite::Key::Semicolon     /* ; */
#define IFN_KEY_EQUAL					::Infinite::Key::Equal			  /* = */
#define IFN_KEY_A							::Infinite::Key::A
#define IFN_KEY_B							::Infinite::Key::B
#define IFN_KEY_C							::Infinite::Key::C
#define IFN_KEY_D							::Infinite::Key::D
#define IFN_KEY_E							::Infinite::Key::E
#define IFN_KEY_F							::Infinite::Key::F
#define IFN_KEY_G							::Infinite::Key::G
#define IFN_KEY_H							::Infinite::Key::H
#define IFN_KEY_I							::Infinite::Key::I
#define IFN_KEY_J							::Infinite::Key::J
#define IFN_KEY_K							::Infinite::Key::K
#define IFN_KEY_L							::Infinite::Key::L
#define IFN_KEY_M							::Infinite::Key::M
#define IFN_KEY_N							::Infinite::Key::N
#define IFN_KEY_O							::Infinite::Key::O
#define IFN_KEY_P							::Infinite::Key::P
#define IFN_KEY_Q							::Infinite::Key::Q
#define IFN_KEY_R							::Infinite::Key::R
#define IFN_KEY_S							::Infinite::Key::S
#define IFN_KEY_T							::Infinite::Key::T
#define IFN_KEY_U							::Infinite::Key::U
#define IFN_KEY_V							::Infinite::Key::V
#define IFN_KEY_W							::Infinite::Key::W
#define IFN_KEY_X							::Infinite::Key::X
#define IFN_KEY_Y							::Infinite::Key::Y
#define IFN_KEY_Z							::Infinite::Key::Z
#define IFN_KEY_LEFT_BRACKET		::Infinite::Key::LeftBracket			/* [ */
#define IFN_KEY_BACKSLASH			::Infinite::Key::Backslash			/* \ */
#define IFN_KEY_RIGHT_BRACKET  ::Infinite::Key::RightBracket		/* ] */
#define IFN_KEY_GRAVE_ACCENT   ::Infinite::Key::GraveAccent		/* ` */
#define IFN_KEY_WORLD_1				::Infinite::Key::World1				/* non-US #1 */
#define IFN_KEY_WORLD_2				::Infinite::Key::World2				 /* non-US #2 */


/* Function keys */
#define IFN_KEY_ESCAPE					::Infinite::Key::Escape
#define IFN_KEY_ENTER					::Infinite::Key::Enter
#define IFN_KEY_TAB						::Infinite::Key::Tab
#define IFN_KEY_BACKSPACE			::Infinite::Key::Backspace
#define IFN_KEY_INSERT					::Infinite::Key::Insert
#define IFN_KEY_DELETE					::Infinite::Key::Delete
#define IFN_KEY_RIGHT					::Infinite::Key::Right
#define IFN_KEY_LEFT						::Infinite::Key::Left
#define IFN_KEY_DOWN					::Infinite::Key::Down
#define IFN_KEY_UP						::Infinite::Key::Up
#define IFN_KEY_PAGE_UP				::Infinite::Key::PageUp
#define IFN_KEY_PAGE_DOWN       ::Infinite::Key::PageDown
#define IFN_KEY_HOME					::Infinite::Key::Home
#define IFN_KEY_END						::Infinite::Key::End
#define IFN_KEY_CAPS_LOCK			::Infinite::Key::CapsLock
#define IFN_KEY_SCROLL_LOCK     ::Infinite::Key::ScrollLock
#define IFN_KEY_NUM_LOCK			::Infinite::Key::NumLock
#define IFN_KEY_PRINT_SCREEN    ::Infinite::Key::PrintScreen
#define IFN_KEY_PAUSE					::Infinite::Key::Pause
#define IFN_KEY_F1							::Infinite::Key::F1
#define IFN_KEY_F2							::Infinite::Key::F2
#define IFN_KEY_F3							::Infinite::Key::F3
#define IFN_KEY_F4							::Infinite::Key::F4
#define IFN_KEY_F5							::Infinite::Key::F5
#define IFN_KEY_F6							::Infinite::Key::F6
#define IFN_KEY_F7							::Infinite::Key::F7
#define IFN_KEY_F8							::Infinite::Key::F8
#define IFN_KEY_F9							::Infinite::Key::F9
#define IFN_KEY_F10						::Infinite::Key::F10
#define IFN_KEY_F11						::Infinite::Key::F11
#define IFN_KEY_F12						::Infinite::Key::F12
#define IFN_KEY_F13						::Infinite::Key::F13
#define IFN_KEY_F14						::Infinite::Key::F14
#define IFN_KEY_F15						::Infinite::Key::F15
#define IFN_KEY_F16						::Infinite::Key::F16
#define IFN_KEY_F17						::Infinite::Key::F17
#define IFN_KEY_F18						::Infinite::Key::F18
#define IFN_KEY_F19						::Infinite::Key::F19
#define IFN_KEY_F20						::Infinite::Key::F20
#define IFN_KEY_F21						::Infinite::Key::F21
#define IFN_KEY_F22						::Infinite::Key::F22
#define IFN_KEY_F23						::Infinite::Key::F23
#define IFN_KEY_F24						::Infinite::Key::F24
#define IFN_KEY_F25						::Infinite::Key::F25

/* Keypad */
#define IFN_KEY_KP_0						::Infinite::Key::KP0
#define IFN_KEY_KP_1						::Infinite::Key::KP1
#define IFN_KEY_KP_2						::Infinite::Key::KP2
#define IFN_KEY_KP_3						::Infinite::Key::KP3
#define IFN_KEY_KP_4						::Infinite::Key::KP4
#define IFN_KEY_KP_5						::Infinite::Key::KP5
#define IFN_KEY_KP_6						::Infinite::Key::KP6
#define IFN_KEY_KP_7						::Infinite::Key::KP7
#define IFN_KEY_KP_8						::Infinite::Key::KP8
#define IFN_KEY_KP_9						::Infinite::Key::KP9
#define IFN_KEY_KP_DECIMAL		::Infinite::Key::KPDecimal
#define IFN_KEY_KP_DIVIDE			::Infinite::Key::KPDivide
#define IFN_KEY_KP_MULTIPLY		::Infinite::Key::KPMultiply
#define IFN_KEY_KP_SUBTRACT		::Infinite::Key::KPSubtract
#define IFN_KEY_KP_ADD				::Infinite::Key::KPAdd
#define IFN_KEY_KP_ENTER				::Infinite::Key::KPEnter
#define IFN_KEY_KP_EQUAL			::Infinite::Key::KPEqual

#define IFN_KEY_LEFT_SHIFT			::Infinite::Key::LeftShift
#define IFN_KEY_LEFT_CONTROL    ::Infinite::Key::LeftControl
#define IFN_KEY_LEFT_ALT				::Infinite::Key::LeftAlt
#define IFN_KEY_LEFT_SUPER			::Infinite::Key::LeftSuper
#define IFN_KEY_RIGHT_SHIFT		::Infinite::Key::RightShift
#define IFN_KEY_RIGHT_CONTROL ::Infinite::Key::RightControl
#define IFN_KEY_RIGHT_ALT			::Infinite::Key::RightAlt
#define IFN_KEY_RIGHT_SUPER		::Infinite::Key::RightSuper
#define IFN_KEY_MENU					::Infinite::Key::Menu