#pragma once

#include "Time.h"
#include"Basement/Renderer/Camera.h"

#include "Basement/Events/ApplicationEvent.h"
#include "Basement/Events/MouseEvent.h"


namespace Basement {

	class CameraController
	{
	public:
		CameraController() = default;
		~CameraController() = default;
	};

	class Camera3DController : public CameraController
	{
	public:
		Camera3DController(glm::vec3 position, float fov, float aspectRatio, float near_, float far_);
		~Camera3DController() = default;

		void Update(Timer dt);
		void HandleEvent(Event& event);

		Camera3D& GetCamera() { return m_Camera; }
		const Camera3D& GetCamera() const { return m_Camera; }
	private:
		glm::vec3 m_Position;
		float m_Fov, m_AspectRatio;
		float m_ZoomLevel;
		Camera3D m_Camera;

		glm::mat4 m_Rotation;
		float m_RotationSpeed;

		float m_MoveSpeed;
		float m_MoveSpeedFactor;
	};

	class Camera2DController : public CameraController
	{
	public:
		Camera2DController(float aspectRatio, bool enableRotaion = false);
		~Camera2DController() = default;

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