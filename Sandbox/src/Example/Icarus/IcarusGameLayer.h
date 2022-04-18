#pragma once

#include "Infinite.h"

#include "IcarusLevel.h"
#include <imgui/imgui.h>

class IcarusGameLayer : public Infinite::Layer
{
public:
	IcarusGameLayer();
	virtual ~IcarusGameLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Infinite::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Infinite::Event& e) override;
	bool OnMouseButtonPressed(Infinite::MouseButtonPressedEvent& e);
	bool OnWindowResize(Infinite::WindowResizeEvent& e);
private:
	void CreateCamera(uint32_t width, uint32_t height);
private:
	Infinite::Scope<Infinite::OrthographicCamera> m_Camera;
	IcarusLevel m_Level;
	ImFont* m_Font;
	float m_Time = 0.0f;
	bool m_Blink = false;

	enum class GameState
	{
		Play = 0, MainMenu = 1, GameOver = 2
	};

	GameState m_State = GameState::MainMenu;
};