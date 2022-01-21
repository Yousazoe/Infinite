---
title: Logging
tags:
  - Cpp
  - Game Engine
categories: 游戏引擎开发 (Game Engine Series)
abbrlink: 5285b66
date: 2021-02-09 20:37:23
banner_img:
index_img:
comment:
translate_title:
top:
---





![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/008eGmZEly1gnhkcmtqlcj312w0p07wh.jpg)

<div align=center>
  <font size="3">
    <i>
      <a href=" https://www.behance.net/gallery/83676833/Voxel-Commodore-64?tracking_source=search_projects_recommended%7Cvoxel">Voxel Commodore 64</a> by 
      <a href=" https://www.behance.net/jaskoadam18e5">Jaskó Ádám</a>
    </i>
  </font>
</div>



### 引言

这个系列我们将从零开始开发一个自己的游戏引擎。上次我们为程序定义了实际的切入点，今天我们来看一下日志记录以及如何使用。

<!--more-->



### 日志记录

日志只是我们记录的一种基本方式，我们现在实际需要一种能和我们的引擎交流的途径从而正确的使用它。例如我们启动引擎，打印系统初始化信息、文件是否成功打开、着色器编译成功......有太多的信息需要反馈给我们，优秀的软件是面向客户的。

当我们编写初具规模的软件，随着代码量的增长出错时事情会变得很复杂。我们想知道实际发生了什么，当应用构建的越来越复杂，揪出错误的成本就会越高，这时我们真的很想知道到底是什么事情发生了、什么事情是计算机已经完成了以及什么状态是正确的......

这就是日志记录大展身手的地，它是我们在编写代码时实际使用的一种方式。通过日志我们与自己沟通交流：我尝试做到这一点；这发生了什么；这是什么状态；这是怎么回事；这是我接下来要做的；这是一个错误......我们可以了解到很多事情例如警告等等。除了与自己沟通，我们还想知道有关信息来自何处以及该消息的严重性。关于消息的严重性我们希望不只是在控制台打印，可以用更偏向于图像化的方式去展现，比如红色代表错误、黄色是警告、普通的东西是绿色、不重要的东西可能是灰色等等......



### 第三方库

日志系统实际上非常复杂，如果真的要做好需要费很大功夫，而这并不是我们做游戏引擎的初衷。所以我们使用第三方库`spdlog`：

![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/image-20210209230839648.png)

检查一下MIT许可，我们只需要保留版权声明即可：

![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/image-20210209232330707.png)

打开命令行进入引擎文件夹所在，克隆该项目到一个新建的第三方库文件夹`vendor`将其作为子模块添加到我们的项目中：

```bash
git submodule add https://github.com/gabime/spdlog Infinite/vendor/spdlog
```



成功克隆之后回到`Visual Studio`中的`Infinite`和`Sandbox`中属性添加附加包含目录：

```
$(SolutionDir)Infinite\vendor\spdlog\include;
```



![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/image-20210209232444341.png)





### 封装

下面的工作基本上是封装了，我们虽然使用spd作为日志，但我们显然不能和spd日志一模一样，我们自己的引擎应该有类似于`Infinite's log`这样的东西。

首先我们需要更改API，不能看起来像他们的API。他们的API相当丰富，但我们只需要其中的一部分来构建一个引擎的日志记录系统，全局宏是个不错的主意。另一个原因在于如果我们之后用另一个日志库替换这个日志库，我们理应无需修改任何客户端的内容即可完成替换，也就是常说的低耦合。

在`Infinite\src`中新建一个名为`Log`的C++类，将内容包含在名称空间`Infinite`中：

```c++
#pragma once

#include "Core.h"

namespace Infinite {
  class INFINITE_API Log
  {
  public:
    Log() {}
    ~Log() {}
  };
}
```



```c++
#include "Log.h"

namespace Infinite {
  Log::Log() {}
  Log::~Log(){}
}
```



现在我们需要声明一个初始化函数`Init()`和一些内联静态共享指针的引用。通过声明我们将拥有内核记录器`CoreLogger`以及客户端记录器`ClientLogger`。



```c++
#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"

namespace Infinite {
  class INFINITE_API Log
  {
  public:
    static void Init();
    
    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger;  }
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    
  private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
  };
}
```



现在我们要去设置这些记录器，查阅spd库文档我们选择这种模式：以正确的方式上色然后加上时间戳和记录名，最后显示实际的日志消息。

```c++
spdlog::set_pattern("%^[%T] %n: %v%$");
```



对其他参数进行设置：

```c++
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Infinite {
  std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
  std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
  
  void Log::Init() {
    spdlog::set_pattern("%^[%T] %n: %v%$");
    s_CoreLogger = spdlog::stdout_color_mt("Infinite");
    s_CoreLogger->set_level(spdlog::level::trace);
    
    s_ClientLogger = spdlog::stderr_color_mt("APP");
    s_ClientLogger->set_level(spdlog::level::trace);
  }
}
```



引入`Loh.h`头文件避免出现编译错误：

```diff
#pragma once

//For use by Infinite Applications

#include "Application.h"
+ #include "Log.h"
#include "Core.h"

#include <stdio.h>

//--------------------- Entry Point ---------------------//
#include "EntryPoint.h"
//-------------------------------------------------------//
```



返回`EntryPoint.h`，添加初始化函数和对刚才设置的一些测试：

```diff
#pragma once

#ifdef IFN_PLATFORM_WINDOWS

extern Infinite::Application* Infinite::CreateApplication();

int main(int argc,char** argv) {
+ Infinite::Log::Init();
+ Infinite::Log::GetCoreLogger()->warn("Initialized Log!");
+ Infinite::Log::GetClientLogger()->info("Hello!");

 printf("Infinite Engine");
 auto app = Infinite::CreateApplication();
 app->Run();
 delete app;
}

#endif
```



运行前需要像之前一样把`Infinite`的dll覆盖到`Sandbox`中，编译运行就会有日志打印出来。测试是成功了，但我们希望更进一步，将这些spd日志的调用用`#define`封装成宏：



```diff
#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"

namespace Infinite {
  class INFINITE_API Log
  {
  public:
    static void Init();
    
    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger;  }
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    
  private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
  };
}

//Core log macros
+ #define IFN_CORE_FATAL(...) ::Infinite::Log::GetCoreLogger()->fatal(__VA_ARGS__)
+ #define IFN_CORE_ERROR(...) ::Infinite::Log::GetCoreLogger()->error(__VA_ARGS__)
+ #define IFN_CORE_WARN(...)  ::Infinite::Log::GetCoreLogger()->warn(__VA_ARGS__)
+ #define IFN_CORE_INFO(...)  ::Infinite::Log::GetCoreLogger()->info(__VA_ARGS__)
+ #define IFN_CORE_TRACE(...) ::Infinite::Log::GetCoreLogger()->trace(__VA_ARGS__)

//Client log macros
+ #define IFN_FATAL(...)      ::Infinite::Log::GetClientLogger()->fatal(__VA_ARGS__)
+ #define IFN_ERROR(...)      ::Infinite::Log::GetClientLogger()->error(__VA_ARGS__)
+ #define IFN_WARN(...)       ::Infinite::Log::GetClientLogger()->warn(__VA_ARGS__)
+ #define IFN_INFO(...)       ::Infinite::Log::GetClientLogger()->info(__VA_ARGS__)
+ #define IFN_TRACE(...)      ::Infinite::Log::GetClientLogger()->trace(__VA_ARGS__)
```



回到`EntryPoint.h`用刚刚写的宏来替换之前的调用：

```diff
#pragma once

#ifdef IFN_PLATFORM_WINDOWS

extern Infinite::Application* Infinite::CreateApplication();

int main(int argc,char** argv) {
	Infinite::Log::Init();
-	Infinite::Log::GetCoreLogger()->warn("Initialized Log!");
-	Infinite::Log::GetClientLogger()->info("Hello!");

+	IFN_CORE_WARN("Initialized Log!");
+	int a = 5;
+	IFN_INFO("Hello! {0}",a);

	printf("Infinite Engine");
	auto app = Infinite::CreateApplication();
	app->Run();
	delete app;
}

#endif
```



再次运行，编译成功，输出结果与之前一致。

![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/008eGmZEly1gnhki0y17gj30yf0hz74w-20210209212559716-20210209225727053.jpg)