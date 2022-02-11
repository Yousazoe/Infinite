#pragma once

#include "Core.h"
#include "../Window.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "./ImGui/ImGuiLayer.h"

#include "./Renderer/Shader.h"
#include "./Renderer/Buffer.h"
#include "./Renderer/VertexArray.h"

#include "./Renderer/OrthographicCamera.h"

namespace Infinite {
	class INFINITE_API Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<Shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_SquareVertexArray;

		OrthographicCamera m_Camera;
	private:
		static Application* s_Instance;
	};

	//To be define in CLIENT
	Application* CreateApplication();
}