#pragma once

#include "Infinite/Core/Core.h"
#include "Infinite/Events/Event.h"
#include "Infinite/Core/Timestep.h"

namespace Infinite {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender() {}

		const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}
