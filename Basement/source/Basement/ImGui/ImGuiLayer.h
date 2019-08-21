#pragma once

#include "Basement/Layer.h"

#include "Basement/Events/ApplicationEvent.h"
#include "Basement/Events/MouseEvent.h"
#include "Basement/Events/KeyEvent.h"

namespace Basement {

	class BASEMENT_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void Attach() override;
		virtual void Detach() override;
		virtual void Update() override;
		virtual void ProcessEvent(Event& event) override;
	private:
		bool HasMouseButtonPressedEvent(MouseButtonPressedEvent& event);
		bool HasMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);
		bool HasMouseMovedEvent(MouseMovedEvent& event);
		bool HasMouseScrolledEvent(MouseScrolledEvent& event);
		bool HasKeyPressedEvent(KeyPressedEvent& event);
		bool HasKeyReleasedEvent(KeyReleasedEvent& event);
		bool HasKeyTypedEvent(KeyTypedEvent& event);
		bool HasWindowResizeEvent(WindowResizeEvent& event);
	private:
		float m_Time = 0.0f;
	};


}