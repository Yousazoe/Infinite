#include "ifnpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "./Platform/OpenGL/OpenGLVertexArray.h"

namespace Infinite {

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			IFN_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexArray();
		}

		IFN_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


}
