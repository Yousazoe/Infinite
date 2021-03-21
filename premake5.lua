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
