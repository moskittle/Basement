#pragma once

#include "Event.h"

namespace Basement {
	
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(float width, float height)
			: m_Width(width), m_Height(height) {}

		inline float GetWidth() const { return m_Width; }
		inline float GetHeight() const { return m_Height; }

		// Event Category
		virtual int GetCategoryFlags() const override { return EventCategoryApplication; }

		// Event Type
		static EEventType GetStaticType() { return EEventType::WindowResize; }
		virtual EEventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetEventName() const override { return EventTypeName(WindowResize); }

		virtual std::string ToString() const override 
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

	private:
		float m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		// Event Category
		virtual int GetCategoryFlags() const override { return EventCategoryApplication; }

		// Event Type
		static EEventType GetStaticType() { return EEventType::WindowClose; }
		virtual EEventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetEventName() const override { return EventTypeName(WindowClose); }
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		// Event Category
		virtual int GetCategoryFlags() const override { return EventCategoryApplication; }

		// Event Type
		static EEventType GetStaticType() { return EEventType::AppTick; }
		virtual EEventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetEventName() const override { return EventTypeName(AppTick); }
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		// Event Category
		virtual int GetCategoryFlags() const override { return EventCategoryApplication; }

		// Event Type
		static EEventType GetStaticType() { return EEventType::AppUpdate; }
		virtual EEventType GetEventType() { return GetStaticType(); }
		virtual const char* GetEventName() const override { return EventTypeName(AppUpdate); }
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		// Event Category
		virtual int GetCategoryFlags() const override { return EventCategoryApplication; }

		// Event Type
		static EEventType GetStaticType() { return EEventType::AppRender; }
		virtual EEventType GetEventType() { return GetStaticType(); }
		virtual const char* GetEventName() const override { return EventTypeName(AppRender); }
	};
}