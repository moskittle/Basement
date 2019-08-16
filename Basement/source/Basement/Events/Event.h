#pragma once

#include "Basement/Core.h"

#include <string>
#include <functional>

namespace Basement {

	// For the future, a better strategy might be to buffer events in an event bus
	// and process them during the "event" part of the update stage

	enum class EEventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonRelease, MouseButtonMoved, MouseButtonScrolled
	};

	enum EEventCategory
	{
		None = 0,
		EventCategoryApplication           = BIT(0),
		EventCategoryInput                 = BIT(1),
		EventCategoryKeyboard              = BIT(2),
		EventCategoryMouse                 = BIT(3),
		EventCategoryMouseButton           = BIT(4)
	};

// EVENT_CLASS_TYPE macro
#define EventTypeName(type) #type 
	
	class BASEMENT_API Event
	{
		friend class EventDispatcher;
	public:
		virtual int GetCategoryFlags() const = 0;

		virtual EEventType GetEventType() const = 0;
		virtual const char* GetEventName() const = 0;
		virtual std::string ToString() const { return GetEventName(); }

		inline bool IsInCategory(EEventCategory category) { return GetCategoryFlags(); }
	protected:
		bool m_Handled = false;
	};

	class EventDispatcher
	{

	};

}