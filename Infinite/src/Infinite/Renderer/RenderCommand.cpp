#include "ifnpch.h"
#include "Infinite/Renderer/RenderCommand.h"

namespace Infinite {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}