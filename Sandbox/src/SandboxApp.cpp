#include <Infinite.h>

#include "../Platform/OpenGL/OpenGLShader.h"

#include "../imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Infinite::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(Infinite::VertexArray::Create());


		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		Infinite::Ref<Infinite::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Infinite::VertexBuffer::Create(vertices, sizeof(vertices)));
		Infinite::BufferLayout layout = {
			{ Infinite::ShaderDataType::Float3, "a_Position" },
			{ Infinite::ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Infinite::Ref<Infinite::IndexBuffer> indexBuffer;
		indexBuffer.reset(Infinite::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);


		m_SquareVertexArray.reset(Infinite::VertexArray::Create());

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		Infinite::Ref<Infinite::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(Infinite::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVertexBuffer->SetLayout({
			{ Infinite::ShaderDataType::Float3, "a_Position" },
			{ Infinite::ShaderDataType::Float2, "a_TexCoord"}
			});
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Infinite::Ref<Infinite::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(Infinite::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);



		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader = Infinite::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);



		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_FlatColorShader = Infinite::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);


		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");


		m_Texture = Infinite::Texture2D::Create("assets/textures/Checkerboard.png");
		m_InfiniteLogoTexture = Infinite::Texture2D::Create("assets/textures/InfiniteLogo.png");

		std::dynamic_pointer_cast<Infinite::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Infinite::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Infinite::Timestep ts) override
	{
		if (Infinite::Input::IsKeyPressed(IFN_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Infinite::Input::IsKeyPressed(IFN_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Infinite::Input::IsKeyPressed(IFN_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		else if (Infinite::Input::IsKeyPressed(IFN_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;

		if (Infinite::Input::IsKeyPressed(IFN_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		else if (Infinite::Input::IsKeyPressed(IFN_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * ts;


		Infinite::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Infinite::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Infinite::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Infinite::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Infinite::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Infinite::Renderer::Submit(m_FlatColorShader, m_SquareVertexArray, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Infinite::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_InfiniteLogoTexture->Bind();
		Infinite::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		// Infinite::Renderer::Submit(m_Shader, m_VertexArray);

		Infinite::Renderer::EndScene();

	}



	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}


	void OnEvent(Infinite::Event& event) override
	{
	}


private:
	Infinite::ShaderLibrary m_ShaderLibrary;
	Infinite::Ref<Infinite::Shader> m_Shader;
	Infinite::Ref<Infinite::VertexArray> m_VertexArray;

	Infinite::Ref<Infinite::Shader> m_FlatColorShader;
	Infinite::Ref<Infinite::VertexArray> m_SquareVertexArray;
	Infinite::Ref<Infinite::Texture2D> m_Texture, m_InfiniteLogoTexture;

	Infinite::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;
	float m_CameraMoveSpeed = 5.0f;
	float m_CameraRotationSpeed = 3.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Infinite::Application {
public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {}
};

Infinite::Application* Infinite::CreateApplication() {
	return new Sandbox();
}