#include "bmpch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

namespace Basement {

	//---------------------------------------------------
	//---Perspective Camera------------------------------

	Camera3D::Camera3D(glm::vec3 position, float fov, float aspectRatio, float near_, float far_)
	{
		m_Position = position;

		m_Yaw = -90.0f;
		m_Pitch = 0.0f;

		m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

		UpdateCameraDirection(); // Initialize Front, Right and Up vectors

		UpdateViewMatrix();
		
		m_Fov = fov;
		m_AspectRatio = aspectRatio;
		m_Near = near_; m_Far = far_;
		UpdateProjectionMatrix(m_Fov, m_AspectRatio, m_Near, m_Far);
	}

	void Camera3D::UpdateCameraDirection()
	{
		glm::vec3 front = glm::vec3(1.0f);
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Front = glm::normalize(front);

		// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));

		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	}

	void Camera3D::UpdateProjectionMatrix(float fov, float aspectRatio, float near_, float far_)
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, near_, far_);
	}

	void Camera3D::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		UpdateViewMatrix();
	}

	void Camera3D::SetRotation(float yaw, float pitch)
	{
		m_Yaw = yaw;
		m_Pitch = pitch;

		UpdateCameraDirection();
		UpdateViewMatrix();
	}

	void Camera3D::UpdateViewMatrix()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	void Camera3D::SetFov(float fov)
	{
		m_Fov = fov;
		UpdateProjectionMatrix(m_Fov, m_AspectRatio, m_Near, m_Far);
	}


	//---------------------------------------------------
	//---Orthographic Camera-----------------------------

	Camera2D::Camera2D(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f), m_Rotation(0.0f)
	{
	}

	void Camera2D::SetProjectionMatrix(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	}

	void Camera2D::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		UpdateViewMatrix();
	}


	void Camera2D::SetRotation(float rotation)
	{ 
		m_Rotation = rotation;
		UpdateViewMatrix();
	}


	void Camera2D::UpdateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * 
							  glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		
		m_ViewMatrix = glm::inverse(transform);
	}

}