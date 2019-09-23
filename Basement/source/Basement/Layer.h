#pragma once

#include "Basement/Core.h"
#include "Basement/Events/Event.h"
#include "Basement/Core/Time.h"

namespace Basement {

	class Layer
	{
	public:
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer() = default;

		virtual void Attach() {};
		virtual void Detach() {};
		virtual void Update(const Timer& dt) {};
		virtual void RenderImGui() {};
		virtual void HandleEvent(Event& event) {};

		inline const std::string& GetDebugName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};


}