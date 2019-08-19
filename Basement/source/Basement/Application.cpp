#include "bmpch.h"
#include "Application.h"

#include "Basement/Log.h"
#include "Basement/Events/KeyEvent.h"

#include "GLFW/glfw3.h"

namespace Basement {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		mu_Window = std::unique_ptr<Window>(Window::Create());
		mu_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	void Application::Run()
	{
		while (m_IsRunning)
		{
			float div = 256,r = 105/div, g = 190/div, b = 40/div;
			glClearColor(r, g, b, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			mu_Window->Update();
		}
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(CloseWindow));

		BM_CORE_TRACE("{0}", event);
	}

	bool Application::CloseWindow(WindowCloseEvent& event)
	{
		m_IsRunning = false;
		return true;
	}

}