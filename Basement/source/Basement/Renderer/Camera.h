#pragma once

#include <glm/glm.hpp>

namespace Basement {

	class Camera3D
	{
	public:
		Camera3D(glm::vec3 position, float fov, float aspectRatio, float near_, float far_);
		~Camera3D() = default;

		void SetPosition(const glm::vec3& position);
		void SetRotation(float yaw, float pitch);
		void SetFov(float fov);

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

		const glm::vec3& GetFront() const { return m_Front; }
		const glm::vec3& GetUp() const { return m_Up; }
		const glm::vec3& GetRight() const { return m_Right; }
	private:
		void UpdateCameraDirection();
		void UpdateViewMatrix();
		void UpdateProjectionMatrix(float fov, float aspectRatio, float near_, float far_);
	private:
		float m_Fov, m_AspectRatio, m_Near, m_Far;
		float m_Yaw, m_Pitch;	// Euler Angles
		glm::vec3 m_Front, m_Up, m_Right;
		glm::vec3 m_WorldUp;

		glm::vec3 m_Position, m_Rotation;
		glm::mat4 m_ViewMatrix, m_ProjectionMatrix;
	};


	class Camera2D
	{
	public:
		Camera2D(float left, float right, float bottom, float top);
		~Camera2D() = default;

		void SetProjectionMatrix(float left, float right, float bottom, float top);
		
		void SetPosition(const glm::vec3& position);
		void SetRotation(float rotation);

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
	private:
		void UpdateViewMatrix();
	private:
		glm::mat4 m_ViewMatrix, m_ProjectionMatrix;

		glm::vec3 m_Position;
		float m_Rotation;
	};

}