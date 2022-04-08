#pragma once

#include "Infinite.h"

class ExampleLayer : public Infinite::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Infinite::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Infinite::Event& e) override;
private:
	Infinite::ShaderLibrary m_ShaderLibrary;
	Infinite::Ref<Infinite::Shader> m_Shader;
	Infinite::Ref<Infinite::VertexArray> m_VertexArray;

	Infinite::Ref<Infinite::Shader> m_FlatColorShader;
	Infinite::Ref<Infinite::VertexArray> m_SquareVertexArray;

	Infinite::Ref<Infinite::Texture2D> m_Texture, m_InfiniteLogoTexture;

	Infinite::OrthographicCameraController m_CameraController;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};
