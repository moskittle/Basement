#pragma once

#include "Event.h"

#include <sstream>

namespace Basement {

	class BASEMENT_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		virtual int GetCategoryFlags() const override { return EventCategoryInput | EventCategoryKeyboard; };
	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}

		int m_KeyCode;
	};

	class BASEMENT_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatedCount() const { return m_RepeatCount; }
		
		static EEventType GetStaticType() { return EEventType::KeyPressed; }
		virtual EEventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetEventName() const override { return EventTypeName(EEventType::KeyPressed); }
		virtual std::string ToString() const override 
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}
	private:
		int m_RepeatCount;
	};

	class BASEMENT_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}

		static EEventType GetStaticType() { return EEventType::KeyReleased; }
		virtual EEventType GetEventType() const override { GetStaticType(); }
		virtual const char* GetEventName() const override { return EventTypeName(EEventType::KeyReleased); }
		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent" << m_KeyCode;
			return ss.str();
		}
	};
}