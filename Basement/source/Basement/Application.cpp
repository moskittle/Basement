#include "bmpch.h"
#include "Application.h"

#include "Basement/Log.h"
#include "Basement/Events/ApplicationEvent.h"
#include "Basement/Events/KeyEvent.h"

namespace Basement {

	Application::Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1200, 600);
		KeyPressedEvent e1(1, 10);
		BM_TRACE(e);
		BM_INFO(e1);
		while (true);
	}

}