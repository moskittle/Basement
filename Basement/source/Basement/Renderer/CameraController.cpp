#include "bmpch.h"
#include "CameraController.h"

#include "Basement/Core/Input.h"
#include "Basement/Core/KeyCodes.h"
#include "Basement/Events/Event.h"

static const float DefaultMoveSpeed = 20.0f;
static const float DefaultZoomSpeed = 2.0f;
static const float DefaultRotationSpeed3D = 0.3f;

namespace Basement {

	//--------------------------------------------------------------
	//---Perspective Camera Controller------------------------------

	CameraController3D::CameraController3D(glm::vec3 position, float fov, float aspectRatio, float near_, float far_) :
		m_Position(position),
		m_Fov(fov),
		m_ZoomSpeed(DefaultZoomSpeed),
		m_ZoomLevel(1.0f),
		m_Camera(m_Position, m_Fov, aspectRatio, near_, far_),
		m_Yaw(-90.0f),
		m_Pitch(0.0f),
		m_RotationSpeed(DefaultRotationSpeed3D),
		m_CurrentMousePosition(glm::vec2(0.0f)),
		m_LastMousePosition(glm::vec2(0.0f)),
		m_MoveSpeed(DefaultMoveSpeed)
	{
	}

	void CameraController3D::Update(Timer dt)
	{
		// Camera Movement
		if (Basement::Input::IsKeyPressed(BM_KEY_W))
		{
			m_Position += m_Camera.GetFront() * (m_MoveSpeed * dt);
		}
		else if (Basement::Input::IsKeyPressed(BM_KEY_S))
		{
			m_Position -= m_Camera.GetFront() * (m_MoveSpeed * dt);
		}
		if (Basement::Input::IsKeyPressed(BM_KEY_A))
		{
			m_Position -= m_Camera.GetRight() * (m_MoveSpeed * dt);
		}
		else if (Basement::Input::IsKeyPressed(BM_KEY_D))
		{
			m_Position += m_Camera.GetRight() * (m_MoveSpeed * dt);
		}
		if (Basement::Input::IsKeyPressed(BM_KEY_E))
		{
			m_Position += m_Camera.GetUp() * (m_MoveSpeed * dt);
		}
		else if (Basement::Input::IsKeyPressed(BM_KEY_Q))
		{
			m_Position -= m_Camera.GetUp() * (m_MoveSpeed * dt);
		}

		m_Camera.SetPosition(m_Position);
	}

	void CameraController3D::HandleEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(BM_BIND_EVENT_FN(CameraController3D::ScrollMouse));
		dispatcher.Dispatch<MouseMovedEvent>(BM_BIND_EVENT_FN(CameraController3D::DragMouse));
		dispatcher.Dispatch<WindowResizeEvent>(BM_BIND_EVENT_FN(CameraController3D::ResizeWindow));
	}

	bool CameraController3D::ScrollMouse(MouseScrolledEvent& event)
	{
		// TODO: zoom in/out by changine the distance between focal point and camera position
		if (m_Fov >= 1.0f && m_Fov <= 90.0f)
			m_Fov -= (event.GetOffsetY() * m_ZoomSpeed);

		// Fov = [1, 90]
		m_Fov = (m_Fov >= 1.0f) ? m_Fov : 1.0f;
		m_Fov = (m_Fov <= 90.0) ? m_Fov : 90.0f;

		m_Camera.SetFov(m_Fov);

		// Update move and rotate speed
		m_ZoomLevel -= event.GetOffsetY() * 0.2f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_MoveSpeed = DefaultMoveSpeed * m_ZoomLevel;
		m_RotationSpeed = DefaultRotationSpeed3D * m_ZoomLevel;

		return false;
	}

	bool CameraController3D::DragMouse(MouseMovedEvent& event)
	{
		m_CurrentMousePosition = glm::vec2(event.GetX(), event.GetY());
		glm::vec2 offset = m_CurrentMousePosition - m_LastMousePosition;
		offset = (offset.x > 30.0f || offset.y > 30.0f) ? glm::vec2(0.0f) : offset;
		m_LastMousePosition = m_CurrentMousePosition;

		if (Basement::Input::IsMouseButtonPressed(BM_MOUSE_BUTTON_RIGHT))
		{
			m_Yaw += (offset.x * m_RotationSpeed);
			m_Pitch -= (offset.y * m_RotationSpeed);
		}
		m_Camera.SetRotation(m_Yaw, m_Pitch);

		return false;
	}


	bool CameraController3D::ResizeWindow(WindowResizeEvent& event)
	{
		return false;
	}

	//--------------------------------------------------------------
	//---Orthographic Camera Controller-----------------------------

	CameraController2D::CameraController2D(float aspectRatio, bool enableRotaion) :
		m_AspectRatio(aspectRatio),
		m_ZoomLevel(1.0f),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		m_EnableRotation(enableRotaion),
		m_Rotation(0.0f),
		m_Position(glm::vec3(0.0f)),
		m_MoveSpeed(DefaultMoveSpeed),
		m_RotationSpeed(180.0f)
	{
	}

	void CameraController2D::Update(Timer dt)
	{
		// Camera Movement
		if (Basement::Input::IsKeyPressed(BM_KEY_W))
		{
			m_Position.y += m_MoveSpeed * dt;
		}
		else if (Basement::Input::IsKeyPressed(BM_KEY_S))
		{
			m_Position.y -= m_MoveSpeed * dt;
		}
		if (Basement::Input::IsKeyPressed(BM_KEY_A))
		{
			m_Position.x -= m_MoveSpeed * dt;
		}
		else if (Basement::Input::IsKeyPressed(BM_KEY_D))
		{
			m_Position.x += m_MoveSpeed * dt;
		}

		m_Camera.SetPosition(m_Position);

		// Camera Rotation
		if (m_EnableRotation) {
			if (Basement::Input::IsKeyPressed(BM_KEY_Q))
			{
				m_Rotation += m_RotationSpeed * dt;
			}
			else if (Basement::Input::IsKeyPressed(BM_KEY_E))
			{
				m_Rotation -= m_RotationSpeed * dt;
			}

			m_Camera.SetRotation(m_Rotation);
		}
	}

	void CameraController2D::HandleEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(BM_BIND_EVENT_FN(CameraController2D::ScrollMouse));
		dispatcher.Dispatch<WindowResizeEvent>(BM_BIND_EVENT_FN(CameraController2D::ResizeWindow));
	}

	bool CameraController2D::ScrollMouse(MouseScrolledEvent& event)
	{
		m_ZoomLevel -= event.GetOffsetY() * 0.2f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		// Ajust move speed with zoom level
		m_MoveSpeed = DefaultMoveSpeed * m_ZoomLevel;
		return false;
	}

	bool CameraController2D::ResizeWindow(WindowResizeEvent& event)
	{
		m_AspectRatio = event.GetWidth() / event.GetHeight();
		m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}

