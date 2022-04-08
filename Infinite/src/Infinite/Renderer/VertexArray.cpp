#include "ifnpch.h"
#include "Infinite/Renderer/VertexArray.h"

#include "Infinite/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Infinite {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				IFN_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLVertexArray>();
		}

		IFN_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


}
