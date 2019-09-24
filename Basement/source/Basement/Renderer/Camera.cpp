#include "bmpch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

namespace Basement {

	//---------------------------------------------------
	//---Perspective Camera------------------------------

	Camera3D::Camera3D(glm::vec3 position, float fov, float aspectRatio, float near_, float far_)
	{
		glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

		m_Position = position;
		m_ViewMatrix = glm::lookAt(position, position + front, up);
		
		SetProjectionMatrix(fov, aspectRatio, near_, far_);
	}

	void Camera3D::SetProjectionMatrix(float fov, float aspectRatio, float near_, float far_)
	{

		m_ProjectionMatrix = glm::perspective(fov, aspectRatio, near_, far_);
	}

	void Camera3D::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		UpdateViewMatrix();
	}

	void Camera3D::SetRotation(const glm::vec3& rotation)
	{
		m_Rotation = rotation;
		UpdateViewMatrix();
	}

	void Camera3D::UpdateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
		// TODO: Update Rotation
		
		m_ViewMatrix = glm::inverse(transform);
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
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		
		m_ViewMatrix = glm::inverse(transform);
	}

}