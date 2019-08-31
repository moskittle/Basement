#pragma once

#include "Core.h"

#include "Window.h"
#include "Basement/LayerStack.h"
#include "Events/Event.h"
#include "Basement/Events/ApplicationEvent.h"

#include "Basement/ImGui/ImGuiLayer.h"

#include "Basement/Renderer/ShaderProgram.h"
#include "Basement/Renderer/Buffer.h"
#include "Basement/Renderer/VertexArray.h"
#include "Basement/Renderer/Camera.h"


namespace Basement {
	
	class Application
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
		ImGuiLayer* m_ImGuiLayer;
		bool m_IsRunning = true;
		LayerStack m_LayerStack;
	private:
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}