---
title: Precomplied Headers
comment: false
tags:
  - Cpp
  - Game Engine
categories: 游戏引擎开发 (Game Engine Series)
abbrlink: 247d3b60
date: 2021-03-21 10:30:04
type:
banner_img:
index_img:
translate_title:
top:
mathjax:
---





![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/f3f4d382356033.5d1b312d61f67.png)

<div align=center>
  <font size="3">
    <i>
      <a href="https://www.behance.net/gallery/82356033/Gyeonghoeru-pavilion?tracking_source=search_projects_recommended%7Cvoxel">Gyeonghoeru pavilion</a> by 
      <a href="https://www.behance.net/rgznsk">RGZNSK</a>
    </i>
  </font>
</div>



### 引言

上一节我们设置了事件系统，这一节我们在开发初期就开始真正讨论预编译的头文件并实际开始使用预编译的标头。



<!--more-->



### 正文

回到我们的引擎，显然会需要许多标准库：

+ `Event.h`：

  ```c++
  #include <string>
  #include <functional>
  ```

+ `ApplicationEvent.h`：

  ```c++
  #include <sstream>
  ```

+ `Log.h`：

  ```c++
  #include <memory>
  ```

  

我们并没有太多的源文件，这意味着我们无需花费太多时间进行实际编辑，所以现在创建预编译的头文件是个不错的主意。回到VisualStudio，新建`ifnpch.h`头文件和`ifnpch.cpp`源文件，在`ifnpch.cpp`中引入头文件：

```c++
#include "ifnpch.h"
```

回到`ifnpch.h`，我们要做的第一件事是如果我们的平台是Windows，我们必须包含`<windows.h>`：

```c++
#pragma once

#ifdef IFN_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
```



对于所有平台，我们都可以采用类似的方式通过`iddef`包含在此`ifnpch.h`中。除了特定的平台，我们还需要包含一些常见的头文件诸如`iostream`之类的：

```c++
#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <sstream>

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#ifdef IFN_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
```



然后我们回到之前的文件把这些头文件删掉，通过`premake`将`ifnpch.h`与`ifnpch.cpp`包含进来：

```diff
...
project "Infinite"
	...
	
	target ()
	objdir ()

+	pchheader "ifnpch.h"
+	pchsource "Infinite/src/ifnpch.cpp"

...
```



### 调试

事实上VisualStudio也提供给我们手动设置的方式，在属性中的`C/C++`预编译头中选择使用，并将预编译头文件设置为我们的`ifnpch.h`：

![](https://cdn.jsdelivr.net/gh/Yousazoe/picgo-repo/img/image-20210321161618153.png)



最后让`Event.h`、`ApplicationEvent.h`、`Log.h`包含`ifnpch.h`即可。集成预编译头非常容易，这也是大型项目必须要做的事，这样我们就可以开始使用了。

