---
title: Entry Point
tags:
  - Cpp
  - Game Engine
categories: 游戏引擎开发 (Game Engine Series)
abbrlink: 8e968f45
date: 2021-02-08 13:13:31
banner_img:
index_img:
comment:
translate_title:
top:
---





![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/008eGmZEly1gng1yavr2gj31400u07wh.jpg)

<div align=center>
  <font size="3">
    <i>
      <a href=" https://www.behance.net/gallery/112619487/Voxel-Monsters-Inc?tracking_source=search_projects_recommended%7Cvoxel">Voxel Monsters, Inc</a> by 
      <a href=" https://www.behance.net/vidaforuzan">Vida Foruzan</a>
    </i>
  </font>
</div>



### 引言

这个系列我们将从零开始开发一个自己的游戏引擎。上一节我们配置了基本的环境并构建了动态库，还有一个有些粗糙的沙盒应用，这一节主要是关于引擎的入口点开发构建。

<!--more-->



###  头文件封装

我们首先要做的就是创建一个入口点，我们在构建一个应用程序时，它总会有一个入口点除非是某种静态库，甚至动态库都有入口。本质上我们的应用程序需要从某个地方开始，当我们启动我们的产品时会发生什么？一些代码会运行，我们可能在这里需要调试控制，这也是许多现代引擎运行中实际会发生的。

现在我们来回到项目，继续配置：

+ 在`Infinite`-`src`中新建文件夹`Infinite`，它将会成为整个引擎的核心部分
+ 删掉上一节用于测试的`Test.cpp`和`Test.h`
+ 在新建的文件夹`Infinite`中新建头文件`Core.h`
+ 继续在文件夹`Infinite`中新建一个名为`Application`的类，最终有`Application.cpp`和`Application.h`两个文件



在头文件`Application.h`中我们把所有放在名称空间`Infinite`中并添加一个运行函数声明`Run()`：

```c++
#pragma once

namespace Infinite {
  class Application
  {
  public:
    Application();
    virtual ~Application();
    
    void Run();
  };
}
```

需要注意的是这个析构函数是虚拟`virtual`的，所以该类实际上之后将会被继承。



回到`Application.cpp`，我们也用相同的名称空间将其包裹起来：

```c++
#include "Application.h"

namespace Infinite {
  Application::Application() {}
  Application::~Application() {}
  
  void Application::Run() {
    while (true);
  }
}
```

函数内一个无限的循环确保我们的程序能够一直运行下去不会关闭。

还有一个大问题是必须将dll导出，这样才能在沙盒中导入使用此类：

```c++
#pragma once

namespace Infinite {
  class _declspec(dllexport) Application
  {
  public:
    Application();
    virtual ~Application();
    
    void Run();
  };
}
```

回到`Core.h`，对引擎的API进行定义：

```
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
```

接着右键项目`Infinite`属性，在C/C++项中的预处理器中定义几个宏：

```diff
+IFN_PLATFORM_WINDOWS
+IFN_BUILD_DLL
```



在定义好宏之后我们就可以在`Application.h`中进行替换：

+ 引入头文件`Core.h`
+ 用宏替换dll导出函数`_declspec(dllexport)`

```diff
#pragma once

+#include "Core.h"

namespace Infinite {
- class _declspec(dllexport) Application
+ class INFINITE_API Application
{
public:
	Application();
	virtual ~Application(); 
	
	void Run();
	};
}
```



对dll封装好后我们在沙盒中把`Application.cpp`更改为`SandboxApp.cpp`.避免重名引起的误会：

```diff
- namespace Infinite {
- _declspec(dllimport) void Print();
- }

void main() {
	Infinite::Print();
}
```



在`SandboxApp.cpp`中我并不想这样带斜杠的引入头文件：

```c++
#include "Infinite/App"
```

我更喜欢类似于这样可以直接整理的`.h`头文件

```c++
#include <Infinite.h>
```

所以我们回到`Infinite`中新建一个头文件`Infinite.h`，在这里我们将包含`Infinite`的一切，在目前也就是之前的两个头文件：

```c++
#pragma once

//For use by Infinite Applications

#include "Infinite/Application.h"
#include "Infinite/Core.h"
```

最后我们只需要对`Sandbox`的属性页的C/C++中的附加包含目录添加刚才我们写的头文件`Infinite.h`即可：

```
$(SolutionDir)Infinite\src;
```



### 入口点构建

做好一切准备工作之后就可以引入头文件`<Infinite.h>`，接着创建一个`Sandbox`沙盒类，包含基本的构造函数和析构函数：

```c++
#include <Infinite.h>

class Sandbox : public Infinite::Application {
public:
  Sandbox() {}
  ~Sandbox() {}
};

int main() {
  Sandbox* sandbox = new Sandbox();
  sandbox->Run();
  delete sandbox;
}
```

主函数部分我们创建了一个沙盒实例并调用运行函数，最后不要忘记删除实例，这就是沙盒的一个简单却完整的生命周期。

现在如果直接运行会报错，我们需要在文件管理器中手动将`Infinite`的dll拖拽至`Sandbox`覆盖之前的dll文件。回到`Visual Studio`再次运行，弹出命令行窗口，现在我们就有了一个无限运行的应用程序，虽然它不会做任何事情。

接着构建我们的入口点，在`src/Infinite`中新建头文件`EntryPoint.h`，它的主要任务就是把之前用于创建应用程序的`main`函数放置过来：

```c++
#pragma once

#ifdef IFN_PLATFORM_WINDOWS

extern Infinite::Application* Infinite::CreateApplication();

int main(int argc,char** argv) {
  printf("Infinite Engine");
  auto app = Infinite::CreateApplication();
  app->Run();
  delete app;
}

#endif
```

对于`CreateApplication()`我们首先回到`Application.h`添加声明;

```diff
#pragma once

#include "Core.h"

namespace Infinite {
class INFINITE_API Application
{
public:
	Application();
	virtual ~Application();
	
	void Run();
};
	
	//To be define in CLIENT
+	Application* CreateApplication();
}
```

接着我们在`Sandbox.app`中完成函数的真正定义，返回一个沙盒实例：

```diff
#include <Infinite.h>

class Sandbox : public Infinite::Application {
public:
	Sandbox() {}
	~Sandbox() {}
};

+ Infinite::Application* Infinite::CreateApplication() {
+ return new Sandbox();
+ }
```

最后别忘了引入刚才的`EntryPoint`头文件：

```diff
#pragma once

//For use by Infinite Applications

#include "Infinite/Application.h"
#include "Infinite/Core.h"

+ #include <stdio.h>

//--------------------- Entry Point ---------------------//
+ #include "Infinite/EntryPoint.h"
//-------------------------------------------------------//
```

再次运行沙盒，打印出信息则代表成功。现在我们有了实际应用程序的基本入口点，这对我们的引擎非常重要，因为它必须从入口点开始。

我们做了这么多工作，别忘了推送至github哦。