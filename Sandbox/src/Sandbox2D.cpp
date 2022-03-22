#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1200.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = Infinite::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{

}


void Sandbox2D::OnUpdate(Infinite::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Infinite::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Infinite::RenderCommand::Clear();

	Infinite::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Infinite::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Infinite::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	Infinite::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture);
	Infinite::Renderer2D::EndScene();

	// TODO: Add these functions - Shader::SetMat4, Shader::SetFloat4
	// std::dynamic_pointer_cast<Infinite::OpenGLShader>(m_FlatColorShader)->Bind();
	// std::dynamic_pointer_cast<Infinite::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);
}


void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Infinite::Event& e)
{
	m_CameraController.OnEvent(e);
}