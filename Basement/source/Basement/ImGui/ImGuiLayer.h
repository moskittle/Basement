#pragma once

#include "Basement/Layer.h"

namespace Basement {

	class BASEMENT_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();


		virtual void Attach() override;
		virtual void Detach() override;
		virtual void Update() override;
		virtual void ProcessEvent(Event& event) override;
	private:
		float m_Time = 0.0f;
	};


}