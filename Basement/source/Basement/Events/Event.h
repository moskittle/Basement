#pragma once

#include "bmpch.h"
#include "Basement/Core.h"

namespace Basement {

	// For the future, a better strategy might be to buffer events in an event bus
	// and process them during the "event" part of the update stage

	enum class EEventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EEventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

#define EventTypeName(type) #type 

	class BASEMENT_API Event
	{
		friend class EventDispatcher;
	public:
		bool m_Handled = false;

		// Event Category
		virtual int GetCategoryFlags() const = 0;
		// Event Type
		virtual EEventType GetEventType() const = 0;
		virtual const char* GetEventName() const = 0;
		virtual std::string ToString() const { return GetEventName(); }

		inline bool IsInCategory(EEventCategory category) { return GetCategoryFlags(); }
	};

	class EventDispatcher
	{
		template<typename TEventType>
		using EventFn = std::function<bool(TEventType&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event) {}

		template<typename TEventType>
		bool Dispatch(EventFn<TEventType> func)
		{
			if (m_Event.GetEventType() == TEventType::GetStaticType())
			{
				m_Event.m_Handled = func(*(TEventType*)& m_Event);
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}