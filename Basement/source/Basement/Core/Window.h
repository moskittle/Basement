#pragma once

#include "bmpch.h"

#include "Basement/Core/Core.h"
#include "Basement/Events/Event.h"

namespace Basement {
	
	// Interface representing a desktop system based Window
	class Window
	{
	public:
		struct WindowProps
		{
			std::string Title;
			unsigned int Width;
			unsigned int Height;

			WindowProps(const std::string& title = "Basement Engine",
				unsigned int width = 1280,
				unsigned int height = 720)
				: Title(title), Width(width), Height(height) {}
			~WindowProps() = default;
		};

		using EventCallbackFn = std::function<void(Event&)>;

		virtual void Update() = 0;
		
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual unsigned int GetAspectRatio() const = 0;

		
		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};

}