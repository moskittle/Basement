#pragma once

#include "Event.h"

namespace Basement {

	class BASEMENT_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float mouseX, float mouseY)
			: m_MouseX(mouseX), m_MouseY(mouseY) {}

		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; } 

		// Event Type
		static EEventType GetStaticType() { return EEventType::MouseMoved; }
		virtual EEventType GetEventType() const override { GetStaticType(); }
		virtual const char* GetEventName() const override { return EventTypeName(MouseMoved); }
		
		// Event Category
		virtual int GetCategoryFlags() const override { return EventCategoryInput | EventCategoryMouse; }

		virtual std::string ToString() const override 
		{
			std::stringstream ss;
			ss << "MouseMoveEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}
	private:
		float m_MouseX, m_MouseY;
	};

	class BASEMENT_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float offsetX, float offsetY)
			: m_OffsetX(offsetX), m_OffsetY(offsetY) {}
		
		inline float GetOffsetX() { return m_OffsetX; }
		inline float GetOffsetY() { return m_OffsetY; }

		// Event Category
		virtual int GetCategoryFlags() { return EventCategoryInput | EventCategoryMouse; }

		// Event Type
		static EEventType GetStaticType() { return EEventType::MouseScrolled; }
		virtual EEventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetEventName() const override { return EventTypeName(MouseScrolled); }

		virtual std::string ToString()
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_OffsetX << ", " << m_OffsetY;
			return ss.str();
		}
	private:
		float m_OffsetX, m_OffsetY;
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

	class BASEMENT_API MouseButtonPressed : public MouseButtonEvent
	{
	public:
		MouseButtonPressed(int button)
			: MouseButtonEvent(button) {}

		// Event Type
		static EEventType GetStaticType() { return EEventType::MouseButtonPressed; }
		virtual EEventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetEventName() const override { return EventTypeName(MouseButtonPressed); }

		virtual std::string ToString()
		{
			std::stringstream ss;
			ss << "MouseButtonPressed: " << m_Button;
			return ss.str();
		}
	};

	class BASEMENT_API MouseButtonReleased : public MouseButtonEvent
	{
	public:
		MouseButtonReleased(int button)
			: MouseButtonEvent(button) {}

		// Event Type
		static EEventType GetStaticType() { return EEventType::MouseButtonReleased; }
		virtual EEventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetEventName() const override { return EventTypeName(MouseButtonReleased); }

		virtual std::string ToString()
		{
			std::stringstream ss;
			ss << "MouseButtonReleased: " << m_Button;
			return ss.str();
		}
	};
}