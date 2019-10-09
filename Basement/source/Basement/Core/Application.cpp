#include "bmpch.h"
#include "Application.h"

#include "Basement/Core/Log.h"
#include "Basement/Events/KeyEvent.h"
#include "Basement/Core/Input.h"
#include "Basement/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Basement {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		BM_CORE_ASSERT(!s_Instance, "Application already exsists!");
		s_Instance = this;

		m_Window = UniquePtr<Window>(Window::Create());
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

			if (!m_IsMinimized)
			{
				for (auto& layer : m_LayerStack)
				{
					layer->Update(m_Timer);
				}
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
		dispatcher.Dispatch<WindowResizeEvent>(BM_BIND_EVENT_FN(Application::ResizeWindow));
		
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

	bool Application::ResizeWindow(WindowResizeEvent& event)
	{
		if (event.GetWidth() == 0 || event.GetHeight() == 0)
		{
			m_IsMinimized = true;
		}

		m_IsMinimized = false;
		Renderer::ResizeWindow(static_cast<u32>(event.GetWidth()), static_cast<u32>(event.GetHeight()));

		return false;
	}

}