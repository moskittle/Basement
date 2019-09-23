#pragma once

#include "Time.h"
#include"Basement/Renderer/Camera.h"

#include "Basement/Events/ApplicationEvent.h"
#include "Basement/Events/MouseEvent.h"


namespace Basement {

	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio, bool enableRotaion = false);
		~OrthographicCameraController() = default;

		void Update(Timer dt);
		void HandleEvent(Event& event);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
	private:
		bool ScrollMouse(MouseScrolledEvent& event);
		bool ResizeWindow(WindowResizeEvent& event);
	private:
		float m_AspectRatio;
		float m_ZoomLevel;
		OrthographicCamera m_Camera;

		bool m_EnableRotation;
		float m_Rotation;
		glm::vec3 m_Position;

		float m_MoveSpeed;
		float m_MoveSpeedFactor;
		float m_RotationSpeed;
	};

}