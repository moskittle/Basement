#pragma once

#include "Basement/Layer.h"

#include "Basement/Events/ApplicationEvent.h"
#include "Basement/Events/MouseEvent.h"
#include "Basement/Events/KeyEvent.h"

namespace Basement {

	class  ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void Attach() override;
		virtual void Detach() override;
		virtual void RenderImGui() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};


}