#pragma once

#include "Time.h"
#include"Basement/Renderer/Camera.h"

#include "Basement/Events/ApplicationEvent.h"
#include "Basement/Events/MouseEvent.h"


namespace Basement {

	class Camera2DController {
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
		glm::vec3 m_Position;

		float m_MoveSpeed;
		float m_MoveSpeedFactor;
		float m_RotationSpeed;
	};

}