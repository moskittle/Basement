#pragma once

#include "Event.h"

namespace Basement {

	class BASEMENT_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float xPos, float yPos)
			: m_XPos(xPos), m_YPos(yPos) {}

		inline float GetX() const { return m_XPos; }
		inline float GetY() const { return m_YPos; }

		// Event Type
		static EEventType GetStaticType() { return EEventType::MouseMoved; }
		virtual EEventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetEventName() const override { return EventTypeName(MouseMoved); }
		
		// Event Category
		virtual int GetCategoryFlags() const override { return EventCategoryInput | EventCategoryMouse; }

		virtual std::string ToString() const override 
		{
			std::stringstream ss;
			ss << "MouseMoveEvent: " << m_XPos << ", " << m_YPos;
			return ss.str();
		}
	private:
		float m_XPos, m_YPos;
	};

	class BASEMENT_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {}
		
		inline float GetOffsetX() { return m_XOffset; }
		inline float GetOffsetY() { return m_YOffset; }

		// Event Category
		virtual int GetCategoryFlags() const override { return EventCategoryInput | EventCategoryMouse; }

		// Event Type
		static EEventType GetStaticType() { return EEventType::MouseScrolled; }
		virtual EEventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetEventName() const override { return EventTypeName(MouseScrolled); }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
			return ss.str();
		}
	private:
		float m_XOffset, m_YOffset;
	};

	class BASEMENT_API MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return m_Button; }

		// Event Category
		virtual int GetCategoryFlags() const override { return EventCategoryInput | EventCategoryMouseButton; }
	protected:
		MouseButtonEvent(int button)
			: m_Button(button) {}
		int m_Button;
	};

	class BASEMENT_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {}

		// Event Type
		static EEventType GetStaticType() { return EEventType::MouseButtonPressed; }
		virtual EEventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetEventName() const override { return EventTypeName(MouseButtonPressed); }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressed: " << m_Button;
			return ss.str();
		}
	};

	class BASEMENT_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		// Event Type
		static EEventType GetStaticType() { return EEventType::MouseButtonReleased; }
		virtual EEventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetEventName() const override { return EventTypeName(MouseButtonReleased); }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleased: " << m_Button;
			return ss.str();
		}
	};
}