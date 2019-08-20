#include "bmpch.h"
#include "Application.h"

#include "Basement/Log.h"
#include "Basement/Events/KeyEvent.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Basement {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		BM_CORE_ASSERT(!s_Instance, "Application already exsists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(ProcessEvent));
	}

	void Application::Run()
	{
		while (m_IsRunning)
		{
			float div = 256, r = 105/div, g = 190/div, b = 40/div;	// action green	
			glClearColor(r, g, b, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (auto& layer : m_LayerStack)
			{
				layer->Update();
			}

			m_Window->Update();
		}
	}

	void Application::ProcessEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(CloseWindow));

		BM_CORE_TRACE("{0}", event);
		
		for (auto iter = m_LayerStack.end(); iter != m_LayerStack.begin();)
		{
			(*--iter)->ProcessEvent(event);

			if (event.m_Handled)
			{
				break;
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->Attach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->Attach();
	}

	bool Application::CloseWindow(WindowCloseEvent& event)
	{
		m_IsRunning = false;
		return true;
	}

}