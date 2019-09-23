#include "bmpch.h"
#include "CameraController.h"

#include "Basement/Input.h"
#include "Basement/KeyCodes.h"
#include "Basement/Events/Event.h"

static float DefaultMoveSpeed = 3.0f;


//--------------------------------------------------------------
//---Orthographic Camera Controller-----------------------------

Basement::OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool enableRotaion) :
	m_AspectRatio(aspectRatio), m_ZoomLevel(1.0f),
	m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
	m_EnableRotation(enableRotaion), m_Rotation(0.0f),
	m_Position(glm::vec3(0.0f)),
	m_MoveSpeed(DefaultMoveSpeed), m_RotationSpeed(180.0f),
	m_MoveSpeedFactor(1.0f)
{
}

void Basement::OrthographicCameraController::Update(Timer dt)
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

void Basement::OrthographicCameraController::HandleEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseScrolledEvent>(BM_BIND_EVENT_FN(OrthographicCameraController::ScrollMouse));
	dispatcher.Dispatch<WindowResizeEvent>(BM_BIND_EVENT_FN(OrthographicCameraController::ResizeWindow));
}

bool Basement::OrthographicCameraController::ScrollMouse(MouseScrolledEvent& event)
{
	m_ZoomLevel -= event.GetOffsetY() * 0.2f;
	m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
	m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	
	// Ajust move speed with zoom level
	m_MoveSpeedFactor = m_ZoomLevel;
	m_MoveSpeed = DefaultMoveSpeed * m_MoveSpeedFactor;
	return false;
}

bool Basement::OrthographicCameraController::ResizeWindow(WindowResizeEvent& event)
{
	m_AspectRatio = event.GetWidth() / event.GetHeight();
	m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	return false;
}
