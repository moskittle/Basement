#pragma once

#include "bmpch.h"
#include "Basement/Core/Core.h"

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

	class  Event
	{
		friend class EventDispatcher;
	public:
		Event() = default;
		~Event() = default;

		// Event Category
		virtual int GetCategoryFlags() const = 0;
		// Event Type
		virtual EEventType GetEventType() const = 0;
		virtual const char* GetEventName() const = 0;
		virtual std::string ToString() const { return GetEventName(); }

		inline bool IsInCategory(EEventCategory category) { return GetCategoryFlags(); }
		inline bool GetHandleStatus() { return m_IsHandled; }
	private:
		bool m_IsHandled = false;
	};

	class EventDispatcher
	{
	private:
		//template<typename TEventType>
		//using EventFn = std::function<bool(TEventType&)>;
	public:
		EventDispatcher() = default;
		~EventDispatcher() = default;

		EventDispatcher(Event& event)
			: m_Event(event) {}

		template<typename TEventType, typename TEventFunc>
		bool Dispatch(const TEventFunc& func)
		{
			if (m_Event.GetEventType() == TEventType::GetStaticType())
			{
				m_Event.m_IsHandled = func(static_cast<TEventType&>(m_Event));
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