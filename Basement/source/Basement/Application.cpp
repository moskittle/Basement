#include "bmpch.h"
#include "Application.h"

#include "Basement/Log.h"
#include "Basement/Events/ApplicationEvent.h"
#include "Basement/Events/KeyEvent.h"

#include "GLFW/glfw3.h"

namespace Basement {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	void Application::Run()
	{
		while (m_IsRunning)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->Update();
		}
	}

}