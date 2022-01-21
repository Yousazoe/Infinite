---
title: Premake
tags:
  - Cpp
  - Game Engine
categories: 游戏引擎开发 (Game Engine Series)
abbrlink: 9209cb4c
date: 2021-02-12 17:34:35
banner_img:
index_img:
comment:
translate_title:
top:
---



![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/f1a64d28384331.55bd3bf689627.png)

<div align=center>
  <font size="3">
    <i>
      <a href="https://www.behance.net/gallery/28384331/Factory-03?tracking_source=search_projects_recommended%7Cvoxel">Voxel Factory 03</a> by 
      <a href="https://www.behance.net/pixego">pixego</a>
    </i>
  </font>
</div>



### 引言

这个系列我们将从零开始开发一个自己的游戏引擎。上一次我们讨论日志记录时添加了一个名为`spdlog`的日志记录库。今天我们将以`premake`为基础重构项目文件。

<!--more-->



### 必要性

如果我们仅仅是用`Visual Studio`制作我们的项目，在目前的`Windows`平台上看来是ok的，但最大的问题是当我们不得不处理不同的平台例如`Mac`、`Linux`甚至`Android`或`IOS`，我们需要为特定的IDE生成不同的项目文件。

我们要使用的工具叫做`Premake`。这里不使用`CMake`的原因是我认为非常复杂，虽然使用效果好但它有很多格式化的东西，这种复杂对于我们而言不是必要的；而`Premake`在编写预生成文件时使用`Lua`生成项目文件，功能强大并且易于初学者理解。一旦我们定义好`Premake`的格式，事实上我们就可以定义`Infinite`是什么，例如C++我们需要包含这些头文件、这些是包含目录、这些是我们想要链接的库......在`Lua`中定义好我们想要设置的配置，我们就可以只需运行`Premake`，它将为项目生成`Visual Sutdio`或`Xcode`的项目文件。

这就是我们为什么要使用这种项目生成器的原因而不是手动处理所有事情。可以想象手动维护所有内容，每次添加新文件时相关文件都会同时添加到`Mac`和`Windows`，但是我们必须手动进行所有操作，这很痛苦。



### premake-core

在Github中下好发布版本，我选择的是[Premake 5.0 alpha 15](https://github.com/premake/premake-core/releases/tag/v5.0.0-alpha15)

![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/%E5%B1%8F%E5%B9%95%E5%BF%AB%E7%85%A7%202021-02-12%20%E4%B8%8B%E5%8D%885.38.20.png)



下载好之后复制该`.exe`文件，在最初始的`Infinite`目录中新建`vendor`文件夹，这个文件夹与`Infinite\Infinite\vendor`那个存储`spdlog`的文件夹稍有不同，此文件夹适用于项目解决方案中的一切。继续在`vendor`中创建文件夹`bin`，在`bin`中创建`premake`文件夹，复制粘贴刚才的`premake5.exe`文件以及许可证：

```
Infinite
|__ vendor
	|__ bin
		|__ premake
			|__ premake5.exe
			|__ LICENSE.txt
```





### premake5.lua

回到最初始的`Infinite`目录，新建`premake5.lua`，编写代码：

```lua
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

```

+ `workspace`可以当作解决方案；架构就是我们实际上想要处理的平台，我们选择`x64`
+ 配置我们设置了三种：调试`Debug`、发布`Release`和发行`Dist`
+ `outputdir`输出目录：`%{cfg.buildcfg}`即上面的配置；`%{cfg.system}`为系统；`{cfg.architecture}`是刚才的平台



#### Infinite

此部分用于构建`Infinit/Infinite`：

```lua
project "Infinite"
  location "Infinite"
  kind "SharedLib"
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
    "%{prj.name}/vendor/spdlog/include"
  }

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
```

+ `location`项目位置；`kind`项目类型；`language`语言为C++
+ `files`包含文件，列出我们现在想要的文件即`.h`与`.cpp`
+ `includedirs`引入`spdlog`
+ `filter`过滤器表示具有某种配置或程序属性



#### Sandbox

沙盒基本与`Infinite`一致，只需在一些地方稍作修改即可：

```lua
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
    "Infinite/src/Infinite"
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

+ `kind`项目类型变为`ConsleApp`控制台程序
+ `includedirs`引入`spdlog`与`Infinite`
+ `links`链接器与`Infinite`相连

#### 完整源码 

```lua
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

project "Infinite"
  location "Infinite"
  kind "SharedLib"
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
    "%{prj.name}/vendor/spdlog/include"
  }

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
    "Infinite/src/Infinite"
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



### 启动

在`Infinite`目录中新建`GenerateProject.bat`：

```bat
call vendor\bin\premake\premake5.exe vs2019
PAUSE
```

保存退出后我们只需点击此文件便可生成相应的项目，但在此之前我们需要把以下文件全部删除以更新：

```
Infinite
|__ bin
|__ bin-int
|__ Infinite
|		|__ Infinite.vcxproj
|		|__ Infinite.vcxproj.filters
|		|__ Infinite.vcxproj.user
|__ Sandbox
|		|__ Sandbox.vcxproj
|		|__ Sandbox.vcxproj.user
|__ Infinite.sln
```

这些文件`premake`都会帮你生成，所以不必担心。



### 调试

理论上上述步骤完成后再次运行就成功了，但实际操作的时候遇到了以下两个问题;

+ 生成解决方案的目录混乱
+ dll文件无法正常生成导致沙盒项目无法构建



针对第二点我目前的参考了网络的一种解决方案，在`Infinite`属性中的生成后事件添加：

```
xcopy /y "$(OutDir)Infinite.dll" "$(SolutionDir)bin\$(Configuration)-$(Platform)\Sandbox\"
```

系统报错就会停止，但生成的dll文件在另一个平行的文件夹中，原因是`$(Configuration)-$(Platform)`和`%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}`还是不一致，而我浏览了宏也没有找到合适的，最后只能用拖拽的方式暂时将`Infinite.dll`拖回`Sandbox'`，勉强运行成功。



后再次研究，将问题锁定到生成前事件的指令，我认为是指令不明确的原因，更改为`xcopy /Y ...`也可以解决问题。

```
IF EXIST ..\bin\Debug-windows-x86_64\Infinite\Infinite.dll\ (xcopy /Y  ..\bin\Debug-windows-x86_64\Infinite\Infinite.dll ..\bin\Debug-windows-x86_64\Sandbox > nul) ELSE (xcopy /Y ..\bin\Debug-windows-x86_64\Infinite\Infinite.dll ..\bin\Debug-windows-x86_64\Sandbox > nul)
```

