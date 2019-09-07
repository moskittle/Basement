#pragma once

#include "Core.h"

#include "Window.h"
#include "Basement/LayerStack.h"
#include "Events/Event.h"
#include "Basement/Events/ApplicationEvent.h"
#include "Basement/Core/Time.h"

#include "Basement/ImGui/ImGuiLayer.h"

namespace Basement {
	
	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		inline static Application& GetInstance() { return *s_Instance; }

		void Run();
		void HandleEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }
	private:
		bool CloseWindow(WindowCloseEvent& event);
	private:
		Basement::Unique<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_IsRunning = true;
		LayerStack m_LayerStack;
		Timer m_Timer;
	private:
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}