#include "ifnpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Infinite {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}