---
title: Window Abstraction and GLFW
comment: false
tags:
  - Cpp
  - Game Engine
categories: 游戏引擎开发 (Game Engine Series)
abbrlink: 88888a29
date: 2021-03-21 16:57:01
type:
banner_img:
index_img:
translate_title:
top:
mathjax:
---



![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/46ef8e61408483.5a6df91d1f7c6.jpg)

<div align=center>
  <font size="3">
    <i>
      <a href="https://www.behance.net/gallery/61408483/Memories">Memories</a> by 
      <a href="https://www.behance.net/MChahin">Mohamed Chahin</a>
    </i>
  </font>
</div>



### 引言

上一节我们构建了集成的编译头文件，而今天我们将创建一个图形化界面以便我们可以将东西渲染到我们的窗口上。

<!--more-->





许多人第一步就是对其进行图形化。窗口确实非常有用，我们可以渲染文本和显示调试信息，可视化游戏引擎中实际起作用的事物，但游戏引擎中例如事件系统、应用程序系统中有很多看不到的东西实际上已经在运行，而这部分可能需要很长时间才能完成。

现在创建一个窗口，我们将使用一个名为`GLFW`的库，它拥有跨平台属性（例如Windows、Mac、Linux），是一种非常简单使用相关平台API创建窗口的方式。我不想花太多时间才能实际使用Win32 API创建窗口，所以我们选择OpenGL以便可以尽快得到反馈，尽管它在Windows上也许不是最好的（DirectX）。



### 图形库

现在我们来考虑如何在不同平台上实现一个抽象的窗口类，后续有机会我们可以谈谈平台抽象及其工作原理，而今天我只想打开一个窗口并运行。本质上，我所构建的是一个平台文件夹，里面包含特定平台的代码；然后我创建了一个Windows文件夹并在平台文件夹中包含我们的渲染API代码，例如OpenGL、DirectX。

让我们添加`GLFW`库到我们的引擎中：

```bash
git submodule add https://github.com/TheCherno/glfw Infinite/vendor/GLFW
```



克隆之后我们有了`GLFW`，现在我们要做的是将其添加到预制文件中。`IncludeDir`包含了刚才的`GLFW`库，而`include`将预生成文件依赖于包含的依赖：

```lua
-- include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Infinite/vendor/GLFW/include"

include "Infinite/vendor/GLFW"

project "Infinite"
	...
	includedirs
	{
  	...
  	%{IncludeDir.GLFW}
	}
```





另一个比较重要的是将`Infinite`与`GLFW`链接在一起导入库中：

```lua
project "Infinite"
	...
	links
	{
		"GLFW",
		"opengl32.lib"
	}
```



最后完整的预生成文件`premake.lua`：

```diff
workspace "Infinite"
  architecture "x64"
  startproject "Sandbox"

  configurations
  {
    "Debug",
    "Release",
    "Dist"
  }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

+-- Include directories relative to root folder (solution directory)
+IncludeDir = {}
+IncludeDir["GLFW"] = "Infinite/vendor/GLFW/include"

+include "Infinite/vendor/GLFW"

project "Infinite"
  location "Infinite"
  kind "SharedLib"
  language "C++"

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

  pchheader "ifnpch.h"
  pchsource "Infinite/src/ifnpch.cpp"

  files
  {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp"
  }

  includedirs
  {
    "%{prj.name}/src",
    "%{prj.name}/vendor/spdlog/include",
+    "%{IncludeDir.GLFW}"
  }

+  links
+  {
+    "GLFW",
+    "opengl32.lib"
+  }

  filter "system:windows"
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"

    defines
    {
      "IFN_PLATFORM_WINDOWS",
      "IFN_BUILD_DLL"
    }

    postbuildcommands
    {
      ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
    }

  filter "configurations:Debug"
      defines "IFN_DEBUG"
      symbols "On"

  filter "configurations:Release"
      defines "IFN_RELEASE"
      optimize "On"

  filter "configurations:Dist"
      defines "IFN_DIST"
      optimize "On"



project "Sandbox"
  location "Sandbox"
  kind "ConsoleApp"

  language "C++"

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

  files
  {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp"
  }

  includedirs
  {
    "Infinite/vendor/spdlog/include",
    "Infinite/src/Infinite",
    "Infinite/src/Events"
  }

  links
  {
    "Infinite"
  }

  filter "system:windows"
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"

    defines
    {
      "IFN_PLATFORM_WINDOWS"
    }

  filter "configurations:Debug"
      defines "IFN_DEBUG"
      symbols "On"

  filter "configurations:Release"
      defines "IFN_RELEASE"
      optimize "On"

  filter "configurations:Dist"
      defines "IFN_DIST"
      optimize "On"
```





### 窗口类

新建`Infinite/Window.h`抽象窗口，首先声明一些窗口的基本属性`WindowProps`，包括标题`Title`、长度`Width`、宽度`Height`，并将1280*720指定为默认的宽度和高度：

```c++
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Infinite Engine",
			        unsigned int width = 1280,
			        unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};
```





接着声明一个与具体平台无关的抽象窗口类`Window`。`EventCallbackFn`是事件回调函数，包括析构函数大部分都是虚函数，如你所见实际上这只是一个接口，没有任何数据或功能：

```c++
// Interface representing a desktop system based Window
	class INFNITE_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
```



由于与平台无关，所以我们有这个`Create()`函数用于基于平台的实现，为每个平台创建窗口，这也是为什么只有一个`Window.h`头文件而没有`Window.cpp`的原因，它最终会返回诸如`WindowsWindow`、`MacWindow`或`LinuxWindow`任何我们决定称之为的窗口。而如果我们不指定属性，它将返回默认的属性设置。

完整的`Window.h`源码如下：

```c++
#pragma once

#include "hzpch.h"

#include "Infinite/Core.h"
#include "Events/Event.h"

namespace Infinite {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Infinite Engine",
			        unsigned int width = 1280,
			        unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	// Interface representing a desktop system based Window
	class INFINITE_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};

} 
```





打开平台文件夹`Platform`，有`WindowsWindow.cpp`和`WindowsWindow.h`，包含一些简单的实现。我们有一个窗口数据的结构，这是我们下次要讨论的事件，也是`GLFW`所需要的，现在只需要知道这是我们存储所有内容的地方即可：

```c++
struct WindowData
{
    std::string Title;
    unsigned int Width, Height;
    bool VSync;

    EventCallbackFn EventCallback;
};
WindowData m_Data;
```



而`Init()`和`Shutdown()`则是初始化和关闭函数：

```c++
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
```



完整的`WindowsWindow.h`如下：

```c++
#pragma once

#include "Window.h"

#include <GLFW/glfw3.h>

namespace Infinite {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

} 
```





接着我们跳转到`WindowsWindow.cpp`实现。`Create()`方法中拥有创建窗口的功能，创建窗口并返回新的Windows窗口，返回一个指针将其存储：

```c++
Window* Window::Create(const WindowProps& props)
{
	return new WindowsWindow(props);
}
```



析构函数没有东西的原因是我们并不一定需要销毁窗口，我们后续可能需要创建多个窗口：

```c++
WindowsWindow::~WindowsWindow()
{
	Shutdown();
}
```



我们创建窗口，构造函数调用`Init()`并在其中进行设置，记录窗口的属性并用日志打印：

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

			s_GLFWInitialized = true;
		}

  	......
	}
```



其中`IFN_CORE_ASSERT`在`Core.h`中定义断言，它会检查特定条件并记录日志，如果失败则导致调试，类似于在该行设置断点：

```c++
#ifdef IFN_ENABLE_ASSERTS
    #define IFN_ASSERT(x, ...) { if(!(x)) { IFN_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #define IFN_CORE_ASSERT(x, ...) { if(!(x)) { IFN_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define IFN_ASSERT(x, ...)
    #define IFN_CORE_ASSERT(x, ...)
#endif
```



后面就是`glfwCreateWindow()`创建`GLFW`窗口，`glfwSetWindowUserPointer()`设置一个窗口用户的指针和窗口数据结构体：

```c++
m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
glfwMakeContextCurrent(m_Window);
glfwSetWindowUserPointer(m_Window, &m_Data);
SetVSync(true);
```



`Shutdown()`销毁我们刚刚添加的窗口，`OnUpdate()`更新交换缓冲区。GLFW在默认情况下使用两个缓冲区。 这意味着每个窗口有两个渲染缓冲区：前缓冲区和后缓冲区，前缓冲区是正在显示的缓冲区，后缓冲区是即将显示的缓冲区。当整个帧已经被渲染时，缓冲器需要彼此交换，因此后缓冲器变为前缓冲器，反之亦然。

`GLFW`需要定期与窗口通信，以便接收事件。 事件处理必须在有可见窗口的情况下进行，并且通常在缓冲区交换后每帧执行一次。有两种方法用于处理挂起的事件; 轮询和等待，当你制作游戏或是动画时，尽量使用轮询。 如果相反，你需要在产生事件后才渲染，可是通过等待来处理事件，即`glfwWaitEvent()`，比如制作编辑器的时候，使用等待可以节省大量硬件资源。

```c++
	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
```

交换间隔表示交换缓冲区之前等待的帧数，通常称为vsync。 默认情况下，交换间隔为0，但因为屏幕每秒只更新60-75次，所以大部分的画面不会被显示。而且，缓冲区有可能在屏幕更新的中间交换，出现屏幕撕裂的情况。所以，可以将该间隔设为1，即每帧更新一次。 它可以设置为更高的值，但这可能导致输入延迟。

完整的`WindowsWindow.cpp`如下：

```c++
#include "ifnpch.h"
#include "WindowsWindow.h"

namespace Infinite {

	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

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
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		IFN_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			IFN_CORE_ASSERT(success, "Could not intialize GLFW!");

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

} 
```





### 应用

我们再次进入`Application.h`，这里与平台无关。我将创建一个唯一的指针`m_Window`，该指针保存将一个唯一的窗口类：

```diff
#pragma once

#include "Core.h"
#include "Events/Event.h"
+#include "Window.h"

namespace Infinite {
	class INFINITE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
+	private:
+		std::unique_ptr<Window> m_Window;
+		bool m_Running = true;
	};

	//To be define in CLIENT
	Application* CreateApplication();
} 
```



在`Application.cpp`的构造函数中创建这个唯一的指针，这意味着当应用终止时我们不必自己删除窗口。应用显然是一种单例，我们只有一个完整的应用程序：

```diff
#include "ifnpch.h"
#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "Log.h"

+#include <GLFW/glfw3.h>

namespace Infinite {
	Application::Application() 
	{
+		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application() {}

	void Application::Run() {

-		WindowResizeEvent e(1280, 720);
-		if (e.IsInCategory(EventCategoryApplication))
-		{
-			IFN_TRACE(e);
-		}
-		if (e.IsInCategory(EventCategoryInput))
-		{
-			IFN_TRACE(e);
-		}

+		while (m_Running)
+		{
+			glClearColor(1, 0, 1, 1);
+			glClear(GL_COLOR_BUFFER_BIT);
+			m_Window->OnUpdate();
+		}

-		while (true);
	}
}
```



### 调试

运行，我们会得到一个粉色的窗口：

![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/image-20210528195756909.png)
