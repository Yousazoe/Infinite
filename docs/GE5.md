---
title: Event System
tags:
  - Cpp
  - Game Engine
categories: 游戏引擎开发 (Game Engine Series)
abbrlink: fe6918d5
date: 2021-02-14 17:53:29
banner_img:
index_img:
comment:
translate_title:
top:
---







![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/Eq5wAQ1XMAU2cvz.jpeg)

<div align=center>
  <font size="3">
    <i>
      <a href=" https://twitter.com/m4ndrill">Twitter@m4ndrill</a>
    </i>
  </font>
</div>



### 引言

上一节我们对项目进行重构生成。这节我们将为我们的引擎创建一个事件系统，以便我们处理窗口事件。

<!--more-->



### 架构

我们希望当我们实际创建一个窗口时，我们可以非常轻松地处理所有事情。以前可能先编写整个窗口系统然后再处理事件，但现在最好是先完成事件系统，当我们编写到窗口时已经可以调度所有事件。

目前我们有一个叫`Application`做应用程序的东西，这意味着它实际上包含了使一切保持运行的循环并且一直不断更新我们的游戏。不仅如此，它还要作为事件的枢纽需要接收事件，最终将它们分派到一个`Layer`层中，现在的游戏层是我们以后会讨论的话题所以不会过多地提及它们，因为我们将为此有单独的章节。总之现在事件最终传播到层以便它们可以处理。

![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/image-20210222152835350.png)

根据我们的应用程序，需要有一个应用程序能够接收事件，所以我们来画一些窗口，然后看看这种交流将如何进行。我们的窗口类代表了我们实际构成的窗口，我们的应用程序就像人们熟知的Windows窗口一样，每次我们在此窗口收到事件时是否有人调整了大小、点击关闭按钮、某种鼠标移动事件等等。无论何时每当这些事情发生时，在我们的窗口库或Win32 API都会收到一个事件，然后一旦该事件发生在该窗口类中，我们需要一种将其传达给应用程序的方式。本质上就是窗口类收到一个事件回调，然后构造一个Infinite事件并以某种方式传播回去。

![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/image-20210222153049788.png)

但我们实际上并不想绑定我们的应用程序到窗口类，所以窗口类不应该知道我们的应用程序。所有的应用程序类都会创建一个窗口，所以我们实际上需要做的是将所有这些事件信息发送给应用程序。

接着我们来谈谈其中的细节。首先是Infinite事件，我们需要一个事件类包含所有对事件系统而言所需的信息。例如鼠标右击，我移动了鼠标并在某个位置单击了它，我们需要什么类型的信息呢？需要确定鼠标点击的X&Y坐标以及哪个按钮被按下，然后我们可以处理该信息，这就是所谓的鼠标按下事件。

![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/image-20210222153938439.png)

我们创建一个鼠标右键事件后，需要一种方法将数据发送到应用程序以便我们后续的操作。我们希望这个应用程序为窗口提供回调，就像一个简单的函数指针一样：当我们由应用程序创建一个窗口时我们还将设置一个回调到该窗口类的事件回调，这样每次此窗口得到一个事件，它可以检查回调是否存在。



![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/image-20210222162405638.png)



窗口实际上并不了解应用程序，但如果我们设置了回调窗口，窗口将以这种方式调用此功能。当接收鼠标按钮按下事件时，我们用堆栈上构造它并立即调用此函数，将来我们可以创建类似缓冲事件的系统。



### 事件系统

> Events in Infinite are currently blocking, meaning when an event occurs it
> immediately gets dispatched and must be dealt with right then an there.
> For the future, a better strategy might be to buffer events in an event
> bus and process them during the "event" part of the update stage.



现在的设计方式是没有缓冲的，这类事件不会立刻推迟。之后我们可能会设计为更合理的将事件中的信息推入某种队列或者缓冲，它们会推迟到我们实际经历该事件之前不会立即发生。

首先我们有一个事件类型的枚举类`EventType`拥有不同类型的事件类型，我们将其隔行拆分，包含了按键事件、鼠标事件等等这些内容，这些实际事件在其相关类型的文件中实现，也就是应用程序事件`ApplicationEvent.h`。

```c++
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};
```



将事件分类的原因是因为我们想过滤某些事件，换句话说，从应用程序到某些事件类我正在接收所有的事件，但我只关心键盘事件是否正确。比如一个非常简单的场景，现在我们想记录每个键盘事件或鼠标事件，所以我们必须去检查一下前面的事件：被按下或释放、移动或滚动......

```c++
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication    = BIT(0),
		EventCategoryInput          = BIT(1),
		EventCategoryKeyboard       = BIT(2),
		EventCategoryMouse          = BIT(3),
		EventCategoryMouseButton    = BIT(4)
	};
```

而`BIT(x)`则在`Infinite/Core.h`中定义：

```diff
#pragma once

#ifdef IFN_PLATFORM_WINDOWS
	#ifdef IFN_BUILD_DLL
		#define INFINITE_API _declspec(dllexport)
	#else
		#define INFINITE_API _declspec(dllimport)
	#endif
#else
	#error Infinite only support Windows
#endif
+#define BIT(x)(1 << x)
```

事件可以分为多个类别，例如`Keyboard`、`Mouse`、`MouseButton`都是`Input`事件、`MouseButton`是`Mouse`事件......而我们想要将多个类别应用于单个事件类型，因此我们需要创建一个位字段以便我们可以设置多个位。



让我们来看一下实际的事件基类：

```c++
class INFINITE_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	protected:
		bool m_Handled = false;
	};
```

首先是`m_Handled`变量，它用于事件的阻断。比如当我们实际上开始调度事件执行各种操作时，我们决定不希望进一步传播此信息例如鼠标单击事件，如果我们单击了鼠标而鼠标落在了按钮范围内，事件经过处理后我们希望阻断该事件，因为它很可能已经被按钮处理了。

`IsInCategory()`函数用于返回事件的类别，例如鼠标事件。通过给定事件类别我们可以快速过滤掉某些事件，返回`ture`属于或不属于`false`该类别。

其余有很多虚函数，这意味着必须实现它们，例如获取事件类型`GetEventType()`、获取事件名称`GetName()`、获取事件标志`GetCategoryFlags()`等等，目前仍需要这些函数便于调试。如果我们需要更多详细信息，显然返回事件的名称，`ToString()`就会调用`GetName()`打印事件名称，我们来看一个具体的事件类窗口大小调整事件`ApplicationEvent.h`。

```c++
#pragma once

#include "Event.h"

#include <sstream>

namespace Infinite {

	class INFINITE_API WindowResizeEvent : public Event
	{
	public:
		......
      
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}
		......
}
```

`ToString()`在这里进行了重写，在目前不考虑性能的情况下利用字符流`ss`输出“窗口大小调整事件”和长宽来作为一种打印事件信息的调试信息。



### 键事件

当我们在键盘上按一下某种东西时，事件就会起作用。因此如果当我从键盘上释放键时这是一个按键事件，它在某些方面有权限，即已按下或释放的内容的键控代码：

```c++
#pragma once

#include "Event.h"

#include <sstream>

namespace Infinite {

	class INFINITE_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}

		int m_KeyCode;
	};

	class INFINITE_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_RepeatCount;
	};

	class INFINITE_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}
```

无论对键如何交互，其中关键核心的代码在所有事件间是通用的，这就是为什么做一个包含键相关代码的键事件基类。当一个键被释放时，我们需要知道是否有重复事件，这意味着当我按下一个键时，它将向操作系统发送按键事件，然后它会发送连续的重复事件。

举个例子，当我按下`a`键时它会立刻添加字母`a`，然后有一个暂停，再之后就打印一连串`a`：



所以第一个是按键事件，然后是其他事件，本质上是键重复事件。`m_KeyCode`用于存储键，还有一个保护类构造函数`KeyEvent(int keycode)`：

```c++
#pragma once

#include "Event.h"

#include <sstream>

namespace Infinite {

	class INFINITE_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}

		int m_KeyCode;
	};
  ......
}  
```



`KeyEvent`事件是由刚才的`KeyEvent`派生而来。构造函数`KeyPressedEvent(int keycode, int repeatCount)`中`keycode`用于存储键，`repeatCount`则是重复次数，如果它的值为0那么它是第一次按下按键，不是重复事件。

我们在此处还实现了一个字符串函数`ToString()`，它将覆盖基类打印键按下事件与重复次数：

```c++
class INFINITE_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_RepeatCount;
	};
```



我们还有了这个事件类型的宏`EVENT_CLASS_TYPE(KeyPressed)`，该宏`EVENT_CLASS_TYPE`在`Events/Event.h`定义如下，用于返回事件的类型并将其字符串化：

```c++
#define EVENT_CLASS_TYPE(type) 
static EventType GetStaticType() { return EventType::##type; }\
virtual EventType GetEventType() const override { return GetStaticType(); }\
virtual const char* GetName() const override { return #type; }
```



为什么需要这些静态函数呢？我们希望能够在运行时检查事件的类型，所以很显然我们需要一个返回当前事件的函数，但我们不需要按键事件的实例来查看事件的类型，按键事件永远是按键事件。

在`Events/Event.h`中定义了事件的调度`EventDispatcher`，其中`Dispatch()`的功能是检查当前事件类型的调度是否与该参数匹配：

```c++
class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};
```



### 鼠标事件

鼠标事件与前面的键事件类似。构造函数`MouseMovedEvent()`获取鼠标`m_MouseX`、`m_MouseY`坐标，重写`ToString()`用于打印鼠标移动事件以及坐标：

```c++
#pragma once

#include "Event.h"

#include <sstream>

namespace Infinite {

	class INFINITE_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: m_MouseX(x), m_MouseY(y) {}

		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_MouseX, m_MouseY;
	};

```





鼠标的滚动事件继承自`Event`，构造函数`MouseScrolledEvent()`的`m_XOffset`、`m_YOffset`是两个坐标的偏移量，重写`ToString()`打印滚动事件和偏移量：

```c++
class INFINITE_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_XOffset, m_YOffset;
	};
```



其他鼠标释放按下事件同理：

```c++
class INFINITE_API MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		MouseButtonEvent(int button)
			: m_Button(button) {}

		int m_Button;
	};

	class INFINITE_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class INFINITE_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
```





### 应用事件

应用事件`ApplicationEvent.h`中包括了诸如窗口调整大小`WindowResizeEvent`、窗口关闭`WindowCloseEvent`之类的事件，还有更新`AppUpdateEvent`和渲染`AppRenderEvent`等事件声明出来备用：

```c++
#pragma once

#include "Event.h"

#include <sstream>

namespace Infinite {

	class INFINITE_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};

	class INFINITE_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class INFINITE_API AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class INFINITE_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class INFINITE_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}
```





### 调试

编译出现`C1083`错误，无法读取`#include "Events/Event.h"`，调用逻辑是：

```
SandboxApp.cpp -- Infinite.h -- Application.h -- Events/Event.h
```

回到`Events/Event.h`，源码中`#include "Events/Event.h"`并没有报错，输出窗口显示`Infinite`构建成功而`Sandbox`失败，而之前我一直在`Infinite`的项目配置折腾近三周。最后我在视频的下面找到了解决方案：

![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/image-20210317083429466.png)

很可能是`premake.lua`出错导致没办法导入`src`中的文件，但目前我暂时按上面的方法给`Sandbox`配置添加目录`$(SolutionDir)Infinite\src`后，问题成功解决。

![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/image-20210317185442617.png)

