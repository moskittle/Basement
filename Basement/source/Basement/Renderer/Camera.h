#pragma once

#include <glm/glm.hpp>

namespace Basement {

	class Camera 
	{
	public:
		Camera() = default;
		~Camera() = default;
	};

	class Camera3D : public Camera
	{
	public:
		Camera3D(glm::vec3 position, float fov, float aspectRatio, float near_, float far_);
		~Camera3D() = default;

		void SetProjectionMatrix(float fov, float aspectRatio, float near_, float far_);

		void SetPosition(const glm::vec3& position);
		void SetRotation(const glm::vec3& rotation);

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
	private:
		void UpdateViewMatrix();
	private:
		glm::mat4 m_ViewMatrix, m_ProjectionMatrix;

		glm::vec3 m_Position, m_Rotation;

		//float m_Pitch, m_Yaw;
	};


	class Camera2D : public Camera
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