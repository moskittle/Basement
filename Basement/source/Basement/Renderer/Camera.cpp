#include "bmpch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Basement {

	//---------------------------------------------------
	//---Base Camera-------------------------------------
	Camera::Camera()
		: m_Position(glm::vec3(0.0f)), m_ViewProjectionMatrix(glm::mat4(1.0f))
	{
	}

	void Camera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		RecalculateViewMatrix();
	}


	//---------------------------------------------------
	//---Perspective Camera------------------------------
	PerpectiveCamera::PerpectiveCamera()
	{
	}

	void PerpectiveCamera::RecalculateViewMatrix()
	{
	}


	//---------------------------------------------------
	//---Orthographic Camera-----------------------------
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetRotation(const float rotation)
	{ 
		m_Rotation = rotation;
		RecalculateViewMatrix();
	}


	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) 
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}





}