#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Basement/Events/ApplicationEvent.h"
#include "Window.h"

namespace Basement {
	
	class BASEMENT_API Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();
		void OnEvent(Event& event);
	private:
		bool CloseWindow(WindowCloseEvent& event);
		std::unique_ptr<Window> mu_Window;
		bool m_IsRunning = true;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}