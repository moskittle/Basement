#pragma once

#include "Event.h"

#include <sstream>

namespace Basement {
	
	class BASEMENT_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		// Event Category
		virtual int GetCategoryFlags() const override { return EventCategoryApplication; }

		// Event Type
		static EEventType GetStaticType() { return EEventType::WindowClose; }
		virtual EEventType GetEventType() { GetStaticType(); }
		virtual const char* GetEventName() const override { return EnventTypeName(WindowClose); }
	};

	class BASEMENT_API AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		// Event Category
		virtual int GetCategoryFlags() const override { return EventCategoryApplication; }

		// Event Type
		static EEventType GetStaticType() { return EEventType::AppTick; }
		virtual EEventType GetEventType() { GetStaticType(); }
		virtual const char* GetEventName() const override { return EnventTypeName(AppTick); }
	};

	class BASEMENT_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		// Event Category
		virtual int GetCategoryFlags() const override { return EventCategoryApplication; }

		// Event Type
		static EEventType GetStaticType() { return EEventType::AppUpdate; }
		virtual EEventType GetEventType() { GetStaticType(); }
		virtual const char* GetEventName() const override { return EnventTypeName(AppUpdate); }
	};

	class BASEMENT_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		// Event Category
		virtual int GetCategoryFlags() const override { return EventCategoryApplication; }

		// Event Type
		static EEventType GetStaticType() { return EEventType::AppRender; }
		virtual EEventType GetEventType() { GetStaticType(); }
		virtual const char* GetEventName() const override { return EnventTypeName(AppRender); }
	};
}