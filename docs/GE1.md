---
title: Project SetUp
tags:
  - Cpp
  - Game Engine
categories: 游戏引擎开发 (Game Engine Series)
abbrlink: 2b07a0e9
date: 2021-02-06 21:31:03
banner_img:
index_img:
comment:
translate_title:
top:
---

![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/008eGmZEly1gne54jvymsj31000u0thh.jpg)

<div align=center>
  <font size="3">
    <i>
      <a href="https://www.behance.net/gallery/35750281/The-quiet-room-Voxel-art">The quiet room - Voxel art</a> by 
      <a href="https://www.behance.net/sircarma">Antoine Lendrevie</a>
    </i>
  </font>
</div>





### 引言

这个系列我们将从零开始开发一个自己的游戏引擎。这一节主要是关于引擎开发的前期准备和相关开发环境的配置。

<!--more-->



### 创建托管仓库

在Github中新建一个新的仓库，我们这里想用`Infinite`作为引擎的名字，意为“无限”，也希望大家可以用它来将无限的创意付诸实际：

![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/image-20210209230346928.png)



创建好之后我们会有证书和自述文件，简单介绍一下自己的项目即可：

![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/image-20210209230413954.png)

我们已经拥有了所有的这些，可以`git clone`并开始实际创建项目了。





### 创建项目

打开`Visual Studio`，点击`File`-`New`-`Project`，然后创建一个空的C++项目：

![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/image-20210209230516025.png)



现在我们打开它，就可以得到`Infinite`的解决方案了：

![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/image-20210209230539984.png)



右键项目，在文件资源管理器中打开文件夹，我们发现在`Infinite`的文件夹中有一个名为`Infinite`的项目，因此Infinite到处都是。

接下来我们打开命令提示行，进入该文件夹进行`git clone`将其克隆到一个并不在乎的文件夹中：

```bash
git clone https://github.com/Yousazoe/Infinite Git
```



进入文件夹`Git`显示隐藏文件（会多一个`.git`文件夹），将所有的一切拷贝到`Infinite`文件夹中，然后删除原来存放的`Git`文件夹，得到我们的最终结果。

基本上，我们已经设法将git存储库的东西克隆到我们的项目文件夹中，比如可以看到我们通过github存储库创建的许可和自述文件，所以我们手动完成了将解决方案和git存储库放置在同一目录中。



### 静态库 or 动态库

我们继续进行实际的设置。回到`Visual Studio`，我们看到的是`Infinite`项目文件和解决方案，现在我们的解决方案将不仅包含引擎，实际上是Windows上将其构建为动态链接库，至少是一个dll文件。所以我们要实际执行的操作是创建一个可执行文件的应用程序项目，也就是Windows中的一个`.exe`文件，我们要将该引擎实际链接到这个`.exe`文件中。

现在有两种方法可以做到这一点，可以使用静态库或自己使用动态库。我们选择dll动态库的原因是我们可以选择加载和卸载它，这主要是因为我们要拥有很多依赖关系必须链接到我们的库中。如果我们使用静态库基本上意味着我们拥有的依赖库实际上必须将其链接到我们的游戏中，而dll基本上就像是一个`.exe`文件，所以我们实际上可以将所有内容链接到它们中，这样我们的游戏将仅依赖于包含以下内容的单个dll文件：拥有它所需要的一切而不是拥有数量庞大的其他类型的库文件。

本质上，我们最终要做的是将所有内容链接到引擎dll有关依赖项，这意味着它们都需要成为静态库，因此我们将所有这些静态库链接到引擎中dll，然后将引擎dll链接到我们的游戏中。



### 平台和配置

右键属性，进入配置管理器，我们有两种活动解决方案平台：`x86`和`x64`。现在我不是很想支持32位因为现在已经2021年了，32位不是我想考虑的平台，支持可能是出于一些遗留问题的原因可能是个好主意，但我选择拒绝支持。

所以我们编辑删除`x86`平台，确定它已经消失了：

![](https://tva1.sinaimg.cn/large/008eGmZEly1gne50lr7khj30rs0j8tag.jpg)

现在我们拥有一个平台`x64`，接着去修改所有配置中的配置类型，从`.exe`改为`.dll`：

![](https://tva1.sinaimg.cn/large/008eGmZEly1gne50kw6wpj30rs0j877b.jpg)



另外需要修改的是输出目录和中间目录，`Visual Studio`默认的配置并不是最佳的，而我们想把解决方案目录中名为`bin`的文件夹中所有内容都转移到该位置：

```diff
- $(SolutionDir)$(Platform)\$(Configuration)\
+ $(SolutionDir)bin\$(Platform)-$(Configuration)\$(ProjectName)\
```



```diff
+ $(SolutionDir)bin\int\$(Platform)-$(Configuration)\$(ProjectName)\
or
+ $(SolutionDir)bin-int\$(Platform)-$(Configuration)\$(ProjectName)\
```



![](https://tva1.sinaimg.cn/large/008eGmZEly1gne50h7wh0j30yk0j8421.jpg)



下一件事就是右键单击解决方案文件上，点击添加，新建一个空的C++项目，我们将其命名为`Sandbox`沙盒。我们有我们的引擎，但是现在我们需要一个应用程序使用我们的引擎，因此我将其称为“沙盒”。

之后对沙盒进行配置，和之前一样移除对32位平台的支持并更改输出目录和中间目录。需要注意的是和之前不同，沙盒的配置类型是一个`.exe`的应用程序，这里无需修改。





### 设置启动项

最后右键沙盒项目，设置为启动项目。在`Visual Studio`中保存更改，用其它文本编辑器（这里我用的`Atom`）打开解决方案文件`Infinite.sln`，把刚才的这个沙盒项目从列表中的第二事件调整为第一事件：

```diff
﻿
Microsoft Visual Studio Solution File, Format Version 12.00
# Visual Studio Version 16
VisualStudioVersion = 16.0.30804.86
MinimumVisualStudioVersion = 10.0.40219.1
- Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "Infinite", "Infinite\Infinite.vcxproj", "{C3C58C2E-D7C6-4397-B68B-B47C6E62F44E}"
- EndProject
Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "Sandbox", "Sandbox\Sandbox.vcxproj", "{29E362CA-A3D1-4215-A040-7EFF963C7BED}"
- EndProject
+ Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "Infinite", "Infinite\Infinite.vcxproj", "{C3C58C2E-D7C6-4397-B68B-B47C6E62F44E}"
+ EndProject
Global
	GlobalSection(SolutionConfigurationPlatforms) = preSolution
		Debug|x64 = Debug|x64
		Release|x64 = Release|x64
	EndGlobalSection
	GlobalSection(ProjectConfigurationPlatforms) = postSolution
		{29E362CA-A3D1-4215-A040-7EFF963C7BED}.Debug|x64.ActiveCfg = Debug|x64
		{29E362CA-A3D1-4215-A040-7EFF963C7BED}.Debug|x64.Build.0 = Debug|x64
		{29E362CA-A3D1-4215-A040-7EFF963C7BED}.Release|x64.ActiveCfg = Release|x64
		{29E362CA-A3D1-4215-A040-7EFF963C7BED}.Release|x64.Build.0 = Release|x64
		{C3C58C2E-D7C6-4397-B68B-B47C6E62F44E}.Debug|x64.ActiveCfg = Debug|x64
		{C3C58C2E-D7C6-4397-B68B-B47C6E62F44E}.Debug|x64.Build.0 = Debug|x64
		{C3C58C2E-D7C6-4397-B68B-B47C6E62F44E}.Release|x64.ActiveCfg = Release|x64
		{C3C58C2E-D7C6-4397-B68B-B47C6E62F44E}.Release|x64.Build.0 = Release|x64
	EndGlobalSection
	GlobalSection(SolutionProperties) = preSolution
		HideSolutionNode = FALSE
	EndGlobalSection
	GlobalSection(ExtensibilityGlobals) = postSolution
		SolutionGuid = {93C2B59B-B64B-4873-9805-4CBCD84336E4}
	EndGlobalSection
EndGlobal
```

这样沙盒会成为启动项目而不是`Infinite`，如果是`Infinite`则显然不会运行其dll文件。



ok，现在我们已经构建一个基本的结构，那么我们来将两个项目链接在一起。右键单击沙盒，添加引用：

![](https://tva1.sinaimg.cn/large/008eGmZEly1gne50ea03ej30rs0j40tg.jpg)

这意味着当`Infinite`实际编译并生成静态库文件时将链接到沙盒中。它会自动为我们链接引擎库文件，但为什么它是一个lib文件呢？这是因为当我们在`Visual Studio`编译dll文件时，它实际上会生成一个lib文件，其中会包含每种功能的种类，我们实际是从dll文件中导出。





分别为两个项目新建名为`src`的文件夹，在`Infinite`中新建`Test.h`的头文件和`Test.cpp`的测试点并编写测试：

```c++
#pragma once

namespace Infinite {
	_declspec(dllexport) void Print();
}
```



以最基本的打印功能作为测试，输出信息`Welcome To Infinite Engine!`。

```c++
#include "Test.h"
#include <stdio.h>

namespace Infinite {
  void Print() {
    printf("Welcome To Infinite Engine!");
  }
}
```





在沙盒项目中新建`Application.cpp`，对之前的代码进行测试：

```c++
namespace Infinite {
  _declspec(dllimport) void Print();
}

void main() {
  Infinite::Print();
}
```

如果直接`Build`运行的话会报错缺少`Infinite.dll`，这部分的自动化我们之后再考虑，现在手动从文件夹中拖拽到沙盒中，再次运行打印出信息：

```shell
"Welcome To Infinite Engine!"
```

