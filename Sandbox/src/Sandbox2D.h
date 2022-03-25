#pragma once

#include "Infinite.h"

class Sandbox2D : public Infinite::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Infinite::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Infinite::Event& e) override;
private:
	Infinite::OrthographicCameraController m_CameraController;

	// Temp
	Infinite::Ref<Infinite::VertexArray> m_SquareVertexArray;
	Infinite::Ref<Infinite::Shader> m_FlatColorShader;

	Infinite::Ref<Infinite::Texture2D> m_CheckerboardTexture;

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};
