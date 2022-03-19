#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1200.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	m_SquareVertexArray = Infinite::VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	Infinite::Ref<Infinite::VertexBuffer> squareVB;
	squareVB.reset(Infinite::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->SetLayout({
		{ Infinite::ShaderDataType::Float3, "a_Position" }
		});
	m_SquareVertexArray->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Infinite::Ref<Infinite::IndexBuffer> squareIB;
	squareIB.reset(Infinite::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVertexArray->SetIndexBuffer(squareIB);

	m_FlatColorShader = Infinite::Shader::Create("assets/shaders/FlatColor.glsl");
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

	Infinite::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Infinite::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Infinite::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	Infinite::Renderer::Submit(m_FlatColorShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Infinite::Renderer::EndScene();
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