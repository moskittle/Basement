#pragma once

#include "Time.h"
#include"Basement/Renderer/Camera.h"

#include "Basement/Events/ApplicationEvent.h"
#include "Basement/Events/MouseEvent.h"


namespace Basement {

	class CameraController3D
	{
	public:
		CameraController3D(glm::vec3 position, float fov, float aspectRatio, float near_, float far_);
		~CameraController3D() = default;

		void Update(Timer dt);
		void HandleEvent(Event& event);

		Camera3D& GetCamera() { return m_Camera; }
		const Camera3D& GetCamera() const { return m_Camera; }
	private:
		bool ScrollMouse(MouseScrolledEvent& event);
		bool DragMouse(MouseMovedEvent& event);
		bool ResizeWindow(WindowResizeEvent& event);
	private:
		float m_Fov;
		float m_ZoomSpeed;

		glm::vec3 m_Position;
		Camera3D m_Camera;

		float m_Yaw, m_Pitch;
		float m_RotationSpeed;
		glm::vec2 m_CurrentMousePosition;
		glm::vec2 m_LastMousePosition;

		float m_MoveSpeed;
		float m_MoveSpeedFactor;
	};

	class CameraController2D
	{
	public:
		CameraController2D(float aspectRatio, bool enableRotaion = false);
		~CameraController2D() = default;

		void Update(Timer dt);
		void HandleEvent(Event& event);

		Camera2D& GetCamera() { return m_Camera; }
		const Camera2D& GetCamera() const { return m_Camera; }
	private:
		bool ScrollMouse(MouseScrolledEvent& event);
		bool ResizeWindow(WindowResizeEvent& event);
	private:
		float m_AspectRatio;
		float m_ZoomLevel;

		Camera2D m_Camera;

		bool m_EnableRotation;
		float m_Rotation;
		float m_RotationSpeed;

		glm::vec3 m_Position;
		float m_MoveSpeed;
		float m_MoveSpeedFactor;
	};

}