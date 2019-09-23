#include "bmpch.h"
#include "Application.h"

#include "Basement/Log.h"
#include "Basement/Events/KeyEvent.h"
#include "Basement/Input.h"
#include "Basement/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Basement {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		BM_CORE_ASSERT(!s_Instance, "Application already exsists!");
		s_Instance = this;

		m_Window = Unique<Window>(Window::Create());
		m_Window->SetEventCallback(BM_BIND_EVENT_FN(Application::HandleEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(new Basement::ImGuiLayer());
	}



	void Application::Run()
	{
		while (m_IsRunning)
		{
			m_Timer.SetCurrentFrameTime(static_cast<float>(glfwGetTime()));
			m_Timer.CalculateDeltaTime();

			for (auto& layer : m_LayerStack)
			{
				layer->Update(m_Timer);
			}

			m_ImGuiLayer->Begin();
			for (auto& layer : m_LayerStack)
			{
				layer->RenderImGui();
			}
			m_ImGuiLayer->End();
			
			m_Window->Update();
		}
	}

	void Application::HandleEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BM_BIND_EVENT_FN(Application::CloseWindow));
		
		for (auto iter = m_LayerStack.end(); iter != m_LayerStack.begin();)
		{
			(*--iter)->HandleEvent(event);

			if (event.GetHandleStatus())
			{
				break;
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	bool Application::CloseWindow(WindowCloseEvent& event)
	{
		m_IsRunning = false;

		return true;
	}

}