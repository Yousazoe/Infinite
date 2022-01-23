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
IncludeDir["imgui"] = "Infinite/vendor/imgui/include"
IncludeDir["glm"] = "Infinite/vendor/glm"

include "Infinite/vendor/GLFW"
include "Infinite/vendor/GLAD"
include "Infinite/vendor/imgui"

project "Infinite"
  location "Infinite"
  kind "SharedLib"
  language "C++"
  staticruntime "off"

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

  pchheader "ifnpch.h"
  pchsource "Infinite/src/ifnpch.cpp"

  files
  {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp",
    "%{prj.name}/vendor/glm/glm/**.hpp",
    "%{prj.name}/vendor/glm/glm/**.inl"
  }

  includedirs
  {
    "%{prj.name}/src",
    "%{prj.name}/vendor/spdlog/include",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.GLAD}",
    "%{IncludeDir.imgui}",
    "%{IncludeDir.glm}"
  }

  links
  {
    "GLFW",
    "GLAD",
    "imgui",
    "opengl32.lib"
  }

  filter "system:windows"
    cppdialect "C++17"
    systemversion "latest"

    defines
    {
      "IFN_PLATFORM_WINDOWS",
      "IFN_BUILD_DLL",
      "GLFW_INCLUDE_NONE"
    }

    postbuildcommands
    {
      ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
    }

  filter "configurations:Debug"
      defines "IFN_DEBUG"
      runtime "Debug"
      symbols "On"

  filter "configurations:Release"
      defines "IFN_RELEASE"
      runtime "Release"
      optimize "On"

  filter "configurations:Dist"
      defines "IFN_DIST"
      runtime "Release"
      optimize "On"



project "Sandbox"
  location "Sandbox"
  kind "ConsoleApp"
  staticruntime "off"

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
    "Infinite/src/**.cpp",
    "Infinite/vendor/imgui",
    "%{IncludeDir.glm}"
  }

  links
  {
    "Infinite"
  }

  filter "system:windows"
    cppdialect "C++17"
    systemversion "latest"

    defines
    {
      "IFN_PLATFORM_WINDOWS"
    }

  filter "configurations:Debug"
      defines "IFN_DEBUG"
      runtime "Debug"
      symbols "On"

  filter "configurations:Release"
      defines "IFN_RELEASE"
      runtime "Release"
      optimize "On"

  filter "configurations:Dist"
      defines "IFN_DIST"
      runtime "Release"
      optimize "On"
