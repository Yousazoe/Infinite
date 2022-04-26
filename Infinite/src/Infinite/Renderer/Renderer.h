#pragma once

#include "Infinite/Renderer/RenderCommand.h"

#include "Infinite/Renderer/OrthographicCamera.h"
#include "Infinite/Renderer/Shader.h"

namespace Infinite {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));


		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;
	};


}
