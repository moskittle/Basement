#pragma once

#include "Basement/Core.h"
#include "Basement/Events/Event.h"

namespace Basement {

	class Layer
	{
	public:
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer() = default;

		virtual void Attach() {};
		virtual void Detach() {};
		virtual void Update() {};
		virtual void RenderImGui() {};
		virtual void ProcessEvent(Event& event) {};

		inline const std::string& GetDebugName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};


}