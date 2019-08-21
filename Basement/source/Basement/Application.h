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

		inline static Application& GetInstance() { return *s_Instance; }

		void Run();
		void ProcessEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }
	private:
		bool CloseWindow(WindowCloseEvent& event);
		
		std::unique_ptr<Window> m_Window;
		bool m_IsRunning = true;
		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
		Application(const Application&) = delete;
		void operator=(const Application&) = delete;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}