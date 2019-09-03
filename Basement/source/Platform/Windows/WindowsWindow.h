#pragma once

#include "Basement/Window.h"
#include "Basement/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Basement {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow();
	private:
		void Init(const WindowProps& props);
		void SetGLFWCallbacks();
		void Shutdown();
	public:
		virtual void Update() override;

		inline unsigned int GetWidth() const override { return m_WindowData.Width; }
		inline unsigned int GetHeight() const override { return m_WindowData.Height; }
		inline unsigned int GetAspectRatio() const override{ return m_WindowData.Width / m_WindowData.Height; }

		// Window attributes
		virtual inline void SetEventCallback(const EventCallbackFn& callback) override { m_WindowData.EventCallback = callback; }
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

		virtual inline void* GetNativeWindow() const override { return m_Window; }
	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		struct WindowData
		{
			std::string Title = "";
			unsigned int Width = 0, Height = 0;
			bool VSync = false;

			EventCallbackFn EventCallback;
		};

		WindowData m_WindowData;
	};

}