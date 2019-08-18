#include "bmpch.h"
#include "WindowsWindow.h"

#include <GLFW/glfw3.h>

namespace Basement {

	static bool s_GLFWInitialized = false;

	//Window* Window::Create(const WindowProps& props)
	//{
	//	return new WindowsWindow(props);
	//}



	WindowsWindow::WindowsWindow(const WindowProps & props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Update()
	{
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
	}

	bool WindowsWindow::IsVSync() const
	{
		return false;
	}

	void WindowsWindow::Init(const WindowProps & props)
	{
		m_WindowData.Title = props.Title;
		m_WindowData.Width = props.Width;
		m_WindowData.Height = props.Height;

		BM_CORE_INFO("Creating window {0} ({1}x{2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			//bool success = glfwInit();
			//BM_CORE_ASSERT(success, "Could not initialize GLFW!");

		}

	}

	void WindowsWindow::Shutdown()
	{
	}

}