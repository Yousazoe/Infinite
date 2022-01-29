#include "ifnpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>

namespace Infinite {


	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		IFN_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		IFN_CORE_ASSERT(status, "Failed to initialize Glad!");


		IFN_CORE_INFO("OpenGL Info:");
		IFN_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		IFN_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		IFN_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}