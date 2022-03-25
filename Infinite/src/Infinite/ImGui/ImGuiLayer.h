#pragma once

#include "Infinite/Core/Layer.h"

#include "Infinite/Events/KeyEvent.h"
#include "Infinite/Events/MouseEvent.h"
#include "Infinite/Events/ApplicationEvent.h"

namespace Infinite {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;

	};


}