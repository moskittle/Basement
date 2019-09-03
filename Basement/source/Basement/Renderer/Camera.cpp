#include "bmpch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

namespace Basement {

	//---------------------------------------------------
	//---Base Camera-------------------------------------

	Camera::Camera()
		: m_Position(0.0f), m_ViewProjectionMatrix(glm::mat4(1.0f))
	{
	}

	void Camera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		UpdateViewProjectionMatrix();
	}


	//---------------------------------------------------
	//---Perspective Camera------------------------------

	PerpectiveCamera::PerpectiveCamera()
		:m_ProjectionMatrix(glm::perspective(m_FieldOfView, m_AspectRatio, 0.1f, 100.0f)), m_FieldOfView(glm::radians(45.0f)), 
		m_AspectRatio(1280.0f/720.0f), m_ViewMatrix(1.0f), m_Rotation(0.0f)
	{
	}

	inline void PerpectiveCamera::SetRotation(float rotation)
	{
	}

	void PerpectiveCamera::UpdateViewProjectionMatrix()
	{
		// view matrix: position, target, Up
		// construct view matrix:
		// 1. camera direction: cam_dir = normalize(position - target)
		// 2. right = cross(cam_dir, Up)
		// 3. out = cross(right, up)

		//glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(1.0f, 0.3f, 0.5f));
		
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + cameraFront, yAxis);
		//m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}


	//---------------------------------------------------
	//---Orthographic Camera-----------------------------

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f), m_Rotation(0.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		UpdateViewProjectionMatrix();
	}

	void OrthographicCamera::SetRotation(float rotation)
	{ 
		m_Rotation = rotation;
		UpdateViewProjectionMatrix();
	}


	void OrthographicCamera::UpdateViewProjectionMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) 
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}


	//---------------------------------------------------
	//---Camera3D Euler----------------------------------

	Camera3DEuler::Camera3DEuler(glm::vec3 position = glm::vec3(0.0f))
		: m_ProjectionMatrix(glm::perspective(glm::radians(45.0f), 1280.0f / 720.f, 0.1f, 100.0f)), 
		m_Position(position), m_WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_Yaw(-90.0f), m_Pitch(0.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		UpdateViewProjectionMatrix();
	}

	Camera3DEuler::~Camera3DEuler()
	{
	}

	void Camera3DEuler::SetRotation(float rotation)
	{
		m_Yaw = rotation;
		UpdateViewProjectionMatrix();
	}

	void Camera3DEuler::UpdateViewProjectionMatrix()
	{
		//glm::vec3 front(0.0f);
		//front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		//front.y = sin(glm::radians(m_Pitch));
		//front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		//m_Front = glm::normalize(front);
		//m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
		//m_Up = glm::normalize(glm::cross(m_Right, m_Front));
		//m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		//m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		//roll can be removed from here. because is not actually used in FPS camera
		
		//glm::mat4 matRoll = glm::mat4(1.0f);//identity matrix; 
		glm::mat4 matPitch = glm::mat4(1.0f);//identity matrix
		glm::mat4 matYaw = glm::mat4(1.0f);//identity matrix

		//roll, pitch and yaw are used to store our angles in our class
		//matRoll = glm::rotate(matRoll, m_Roll, glm::vec3(0.0f, 0.0f, 1.0f));
		matPitch = glm::rotate(matPitch, m_Pitch, glm::vec3(1.0f, 0.0f, 0.0f));
		matYaw = glm::rotate(matYaw, m_Yaw, glm::vec3(0.0f, 1.0f, 0.0f));

		//order matters
		glm::mat4 rotate = matPitch * matYaw;

		glm::vec3 eyeVector = glm::normalize(m_Position - glm::vec3(0.0f));


		//glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
			//* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), m_Position);

		m_ViewMatrix = rotate * translate;

		m_ProjectionMatrix = glm::perspective(glm::radians(45.0f), 1280.0f / 720.f, 0.1f, 100.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

	}

}