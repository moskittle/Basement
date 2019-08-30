#pragma once

#include "Event.h"

namespace Basement {

	class KeyEvent : public Event
	{
	public:
		KeyEvent() = default;
		~KeyEvent() = default;

		inline int GetKeyCode() const { return m_KeyCode; }

		// Event Category
		virtual int GetCategoryFlags() const override { return EventCategoryInput | EventCategoryKeyboard; };
	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}

		int m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}
		~KeyPressedEvent() = default;

		inline int GetRepeatedCount() const { return m_RepeatCount; }
		
		// Event Type
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

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}
		~KeyReleasedEvent() = default;

		// Event Type
		static EEventType GetStaticType() { return EEventType::KeyReleased; }
		virtual EEventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetEventName() const override { return EventTypeName(EEventType::KeyReleased); }
		
		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode) {}
		~KeyTypedEvent() = default;

		// Event Type
		static EEventType GetStaticType() { return EEventType::KeyTyped; }
		virtual EEventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetEventName() const override { return EventTypeName(EEventType::KeyTyped); }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}
	};

}