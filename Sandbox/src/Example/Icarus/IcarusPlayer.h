#pragma once

#include <Infinite.h>

#include "IcarusColor.h"
#include "IcarusRandom.h"

#include "IcarusParticleSystem.h"

class IcarusPlayer
{
public:
	IcarusPlayer();

	void LoadAssets();

	void OnUpdate(Infinite::Timestep ts);
	void OnRender();

	void OnImGuiRender();

	void Reset();

	float GetRotation() { return m_Velocity.y * 4.0f - 90.0f; }
	const glm::vec2& GetPosition() const { return m_Position; }

	float GetScore() const { return (m_Position.x + 10.0f) / 10.0f; }
private:
	glm::vec2 m_Position = { -10.0f, 0.0f };
	glm::vec2 m_Velocity = { 5.0f, 0.0f };

	float m_EnginePower = 0.5f;
	float m_Gravity = 0.4f;

	float m_Time = 0.0f;
	float m_SmokeEmitInterval = 0.4f;
	float m_SmokeNextEmitTime = m_SmokeEmitInterval;

	IcarusParticleProps m_SmokeParticle, m_EngineParticle;
	IcarusParticleSystem m_ParticleSystem;

	Infinite::Ref<Infinite::Texture2D> m_ShipTexture;
};