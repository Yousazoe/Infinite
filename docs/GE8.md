---
title: Window Events
comment: false
abbrlink: 1bc324b9
date: 2021-05-27 20:49:05
type:
tags:
  - Cpp
  - Game Engine
categories: 游戏引擎开发 (Game Engine Series)
banner_img:
index_img:
translate_title:
top:
mathjax:
---

![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/77cbdc61408483.5a9276f541b0d.png)

<div align=center>
  <font size="3">
    <i>
      <a href="https://www.behance.net/gallery/61408483/Memories">Memories</a> by 
      <a href="https://www.behance.net/MChahin">Mohamed Chahin</a>
    </i>
  </font>
</div>



### 引言

上一节为我们的游戏引擎创建了一个窗口，讨论了很多抽象类以及我们如何才能真正抽象出真正的窗口逻辑API，而今天我们将通过该窗口实际添加事件。

<!--more-->



事件系统是我最喜欢写的东西了，因为它是一种令人满意的链接所有东西的方式。以前我通常先写一个完整的窗口系统，然后处理事件系统，但那没有意义。如果你做好了事件系统，那么当你编写窗口系统的时候你就已经可以完美地调度你的事件了。你可以完全不用事件，编写整个窗口系统和渲染器，一旦你需要一个能用鼠标或键盘移动的相机，然后你发现要做好这个需要一个 `while` 循环，然后写一整个事件系统。

撇开那些，先做事件系统，然后我们构建窗口的时候能很简单地处理所有事情。现在我们有一个 Application 中心类，它包含了保持所有东西运行的循环，让游戏一直保持更新；同时它是所有东西的中心，需要接收事件并且在游戏中的所有 Layer 之间调度，简单的讲就是 Application 要传播事件到 Layer 中以处理事件，所以如何实现呢？

具体地说 Windows 类是实际窗口的表示，是应用程序的组成部分。每当我们收到事件，比如改变了窗口大小、按下了关闭按钮、鼠标移动、键盘事件等等这些事件，每当这些事件发生的时候，我们使用的 Window 库或者 Win32 API 会收到一个事件。然后，一旦窗口类中发生了事件，需要一种方式通讯给 Application，但是我们不希望 Application 依赖 Window，所以 Window 类应当完全不知晓 Application。

Application 要创建 Window，所以显然知晓 Window。但是我不希望 Window 类知道 Application 任何的东西，这不是应该存在的关联，所以我们需要创建一种方法，可以把所有事件发送回到 Application，然后 Application 可以处理它们。由于 Application 创建了 Window，它显然有能力处理整个消息系统。

当窗口中发生了一个事件，Window 类会收到一个事件回调，然后它要构造一个 Infinite 事件，然后用某种方法传给 Application，我们来讨论一下细节。首先是事件，我们需要一种 Event 类能够包含发生事件需要的所有细节，比如一个鼠标事件。我移动鼠标，点击某处，鼠标信息应该包含点击的坐标以及是哪个键被按下，我们需要一种方式把这些数据发送给 Application。



### 事件回调

为了能够在窗口上设置回调函数，所以在窗口内部编写的是之前在抽象窗口`Window.h`中的事件回调函数：一个返回`void`并接收事件引用的函数：

```c++
using EventCallbackFn = std::fuction<void(Event&)>;
```



在`Application.h`中我们声明一个称为`OnEvent(Event& e)`的函数：

```diff
#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"

namespace Infinite {
		virtual ~Application();

		void Run();
+		void OnEvent(Event& e);
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};
```





接着返回`Application.cpp`中实现`OnEvent(Event& e)`函数。我们可以通过标准库中的`bind`实现，它将绑定应用程序事件，回调事件回调进入这个窗口数据结构`EventCallbackFn EventCallback`，这里我们用宏：

```diff
#include "ifnpch.h"
#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"
#include <GLFW/glfw3.h>

namespace Infinite {

+#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application() 
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
+		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application() {}

+	void Application::OnEvent(Event& e)
+ {
+		IFN_CORE_TRACE("{0}", e);
+ }

	void Application::Run() 
	{
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}
}
```



这里需要注意`OnEvent()`函数前面别忘记加`Application::`，函数体内用之前的日志系统打印事件类型和详细信息。

正如刚才所说，我们将从`GLFW`中获取一些回调，因此我们在`WindowsWindow.cpp`中的`Init()`加入注释：`//Set GLFW callbacks`，之后的代码都在这个位置添加：

```c++
void WindowsWindow::Init(const WindowProps& props)
{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		IFN_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);
		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			IFN_CORE_ASSERT(success, "Could not intialize GLFW!");

            glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks
		.........
```



需要注意引入事件类相关的头文件：

```diff
#include "ifnpch.h"
#include "WindowsWindow.h"

+#include "Events/ApplicationEvent.h"
+#include "Events/MouseEvent.h"
+#include "Events/KeyEvent.h"

namespace Infinite {
}
```





### 更改窗口大小

我们首先来编写窗口大小调整的回调，使用 lambda去到`glfwSetWindowSizeCallback()`这个函数。实际的窗口大小很有趣，三个参数分别是窗口和两个窗口长宽整数：

```c++
/*
 *  @param[in] window The window that was resized.
 *  @param[in] width The new width, in screen coordinates, of the window.
 *  @param[in] height The new height, in screen coordinates, of the window.
 */
typedef void (* GLFWwindowsizefun)(GLFWwindow*,int,int)
```



那么我们可以在此处编写实际的实现，把更新后的长度和宽度返给`data`并创建窗口大小调整事件：

```c++
// Set GLFW callbacks
glfwSetWindowSizeCallback(m_Window,[](GLFWwindow* window,int width,int height)
{
                              WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                              data.Width = width;
                              data.Height = height;

                              WindowResizeEvent event(width,height);
                              data.EventCallback(event);
});
```



### 关闭窗口

```c++
typedef void (*GLFWwindowclosefun)(GLFWwindow*)
```



前面是一样的，真正需要做的是创建窗口关闭事件然后分派它：

```c++
glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
{
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

    WindowCloseEvent event;
    data.EventCallback(event);
});
```





### 按键事件

```c++
typedef void (*GLFWkeyfun)(GLFWwindow*,int,int,int,int)
```



`GLFW`为我们提供了不同的行为，我们用`switch`区分：

+ `GLFW_PRESS`：按下按键
+ `GLFW_RELEASE`：松开按键
+ `GLFW_REPEAT`：连击按键

```c++
glfwSetKeyCallback(m_Window,[](GLFWwindow* window,int key,int scancode,int action,int mods){
	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

  switch (action) {
    case GLFW_PRESS:
      {
        KeyPressedEvent event(key,0);
        data.EventCallback(event);
        break;
      }

    case GLFW_RELEASE:
      {
        KeyReleasedEvent event(key);
        data.EventCallback(event);
        break;
      }

    case GLFW_REPEAT:
      {
        KeyPressedEvent event(key,1);
        data.EventCallback(event);
        break;
      }
  }
});
```



### 鼠标点击

```c++
typedef void (* GLFWmousebuttonfun)(GLFWwindow*,int,int,int);
```





```c++
glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action) {
			    case GLFW_PRESS:
			    {
				    MouseButtonPressedEvent event(button);
				    data.EventCallback(event);
				    break;
			    }

			    case GLFW_RELEASE:
			    {
				    MouseButtonReleasedEvent event(button);
				    data.EventCallback(event);
				    break;
			    }
			}
		});
```





### 鼠标滚轮

```c++
typedef void (* GLFWmousebuttonfun)(GLFWwindow*,int,int,int);
```







```c++
glfwSetScrollCallback(m_Window,[](GLFWwindow* window,double xOffset,double yOffset){
  WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

  MouseScrolledEvent event((float)xOffset,(float)yOffset);
  data.EventCallback(event);
});
```





### 鼠标位置

```c++
typedef void (* GLFWcursorposfun)(GLFWwindow*,double,double);
```





```c++
glfwSetCursorPosCallback(m_Window,[](GLFWwindow* window,double xPos,double yPos){
  WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

  MouseMovedEvent event((float)xPos,(float)yPos);
  data.EventCallback(event);
});
```



### GLFW报错

在`WindowsWindow.cpp`中加入`GLFW`报错的回调，打印报错信息：

```diff
namespace Infinite {

	static bool s_GLFWInitialized = false;
+	static void GLFWErrorCallback(int error,const char* description)
+	{
+		IFN_CORE_ERROR("GLFW Error ({0}): {1}",error,description);
+	}

	......
	
	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}
	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}
	void WindowsWindow::Init(const WindowProps& props)
	{
		......
		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			IFN_CORE_ASSERT(success, "Could not intialize GLFW!");

+     glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		......
}
```





回到`Application.h`，声明`OnWindowClose()`窗口关闭函数：

```diff
#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"

namespace Infinite {
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);
	private:
+		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};
```



接着在`Application.cpp`中实现：

```diff
#include "ifnpch.h"
#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"
#include <GLFW/glfw3.h>

namespace Infinite {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application() 
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application() {}

	void Application::Run() {
	void Application::OnEvent(Event& e)
    {
+		EventDispatcher dispatcher(e);
+		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		IFN_CORE_TRACE("{0}", e);
    }

	void Application::Run() 
	{
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

+	bool Application::OnWindowClose(WindowCloseEvent& e)
+	{
+		m_Running = false;
+		return true;
+	}

}
```







### 调试

如果报错请注意作用域（我一开始报错是因为`OnEvent()`）和头文件（部分需要引用事件类头文件）引用即可，下面是运行引擎事件回调测试：



#### 鼠标移动事件

![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/image-20210528195818356.png)



#### 按键事件

![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/image-20210528195841358.png)



#### 窗口大小调整事件

![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/image-20210528195857882.png)



#### 窗口关闭事件

![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/image-20210528195913808.png)



可以看到现在我们能够成功监测引擎应用程序的窗口事件，并在控制台打印信息出来。
