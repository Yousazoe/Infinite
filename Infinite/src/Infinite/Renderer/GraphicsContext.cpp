#include "ifnpch.h"
#include "Infinite/Renderer/GraphicsContext.h"

#include "Infinite/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Infinite {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    IFN_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		IFN_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}