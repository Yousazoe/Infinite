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
IncludeDir["ImGui"] = "Infinite/vendor/imgui"
IncludeDir["glm"] = "Infinite/vendor/glm"

include "Infinite/vendor/GLFW"
include "Infinite/vendor/GLAD"
include "Infinite/vendor/imgui"

project "Infinite"
  location "Infinite"
  kind "StaticLib"
  language "C++"
  staticruntime "on"
  cppdialect "C++17"

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

  defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}


  includedirs
  {
    "%{prj.name}/src",
    "%{prj.name}/vendor/spdlog/include",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.GLAD}",
    "%{IncludeDir.ImGui}",
    "%{IncludeDir.glm}"
  }

  links
  {
    "GLFW",
    "GLAD",
    "ImGui",
    "opengl32.lib"
  }

  filter "system:windows"
    systemversion "latest"

    defines
    {
      "IFN_PLATFORM_WINDOWS",
      "IFN_BUILD_DLL",
      "GLFW_INCLUDE_NONE"
    }

  filter "configurations:Debug"
      defines "IFN_DEBUG"
      runtime "Debug"
      symbols "on"

  filter "configurations:Release"
      defines "IFN_RELEASE"
      runtime "Release"
      optimize "on"

  filter "configurations:Dist"
      defines "IFN_DIST"
      runtime "Release"
      optimize "on"



project "Sandbox"
  location "Sandbox"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++17"
  staticruntime "on"



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
    systemversion "latest"

    defines
    {
      "IFN_PLATFORM_WINDOWS"
    }

  filter "configurations:Debug"
      defines "IFN_DEBUG"
      runtime "Debug"
      symbols "on"

  filter "configurations:Release"
      defines "IFN_RELEASE"
      runtime "Release"
      optimize "on"

  filter "configurations:Dist"
      defines "IFN_DIST"
      runtime "Release"
      optimize "on"
