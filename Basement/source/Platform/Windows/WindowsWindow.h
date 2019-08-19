#pragma once

#include "Basement/Window.h"

#include <GLFW/glfw3.h>

namespace Basement {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		virtual void Update() override;

		inline unsigned int GetWidth() const override { return m_WindowData.Width; }
		inline unsigned int GetHeight() const override { return m_WindowData.Height; }

		// Window attributes
		virtual inline void SetEventCallback(const EventCallbackFn& callback) override { m_WindowData.EventCallback = callback; }
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

		virtual inline void* GetNativeWindow() const override { return m_Window; }

	private:
		void Init(const WindowProps& props);
		void Shutdown();
	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_WindowData;
	};

}