#pragma once

#include "Core.h"

#include "Window.h"
#include "Basement/LayerStack.h"
#include "Events/Event.h"
#include "Basement/Events/ApplicationEvent.h"

namespace Basement {
	
	class BASEMENT_API Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();
		void ProcessEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	private:
		bool CloseWindow(WindowCloseEvent& event);
		
		std::unique_ptr<Window> mu_Window;
		bool m_IsRunning = true;
		LayerStack m_LayerStack;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}