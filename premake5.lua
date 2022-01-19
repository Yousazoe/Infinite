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

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Infinite/vendor/GLFW/include"
IncludeDir["GLAD"] = "Infinite/vendor/GLAD/include"
IncludeDir["ImGui"] = "Infinite/vendor/imgui/include"

include "Infinite/vendor/GLFW"
include "Infinite/vendor/GLAD"
include "Infinite/vendor/imgui"

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
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.GLAD}",
    "%{IncludeDir.ImGui}"
  }

  links
  {
    "GLFW",
    "GLAD",
    "ImGui",
    "opengl32.lib"
  }

  filter "system:windows"
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"

    defines
    {
      "IFN_PLATFORM_WINDOWS",
      "IFN_BUILD_DLL",
      "GLFW_INCLUDE_NONE"
    }

    postbuildcommands
    {
      ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
    }

  filter "configurations:Debug"
      defines "IFN_DEBUG"
      buildoptions "/MDd"
      symbols "On"

  filter "configurations:Release"
      defines "IFN_RELEASE"
      buildoptions "/MD"
      optimize "On"

  filter "configurations:Dist"
      defines "IFN_DIST"
      buildoptions "/MD"
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
    "Infinite/src/Events",
    "Infinite/src/**.h",
    "Infinite/src/**.cpp"
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
      buildoptions "/MDd"
      symbols "On"

  filter "configurations:Release"
      defines "IFN_RELEASE"
      buildoptions "/MD"
      optimize "On"

  filter "configurations:Dist"
      defines "IFN_DIST"
      buildoptions "/MD"
      optimize "On"
