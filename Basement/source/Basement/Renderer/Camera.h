#pragma once

#include <glm/glm.hpp>

namespace Basement {

	//class Camera
	//{
	//public:
	//	Camera();
	//	virtual ~Camera() = default;

	//	void SetPosition(const glm::vec3& position);
	//	inline virtual void SetRotation(float rotation) = 0;
	//	inline const glm::vec3& GetPosition() const	{ return m_Position; }

	//	inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	//	virtual const glm::mat4& GetProjectionMatrix() const = 0;
	//	virtual const glm::mat4& GetViewMatrix() const = 0;
	//	virtual void UpdateViewProjectionMatrix() = 0;

	//protected:
	//	glm::mat4 m_ViewProjectionMatrix;
	//	glm::vec3 m_Position;
	//};


	//class PerpectiveCamera : public Camera
	//{
	//public:
	//	PerpectiveCamera();
	//	~PerpectiveCamera() = default;

	//	inline virtual void SetRotation(float rotation) override;
	//	
	//	virtual const glm::mat4& GetProjectionMatrix() const override { return m_ProjectionMatrix; }
	//	virtual const glm::mat4& GetViewMatrix() const override { return m_ViewMatrix; }
	//	inline const float& GetRotation() const { return m_Rotation; }

	//	virtual void UpdateViewProjectionMatrix() override;	// TODO: to be defined
	//private:
	//	float m_FieldOfView;
	//	float m_AspectRatio;
	//	float m_Rotation;

	//	glm::mat4 m_ProjectionMatrix;
	//	glm::mat4 m_ViewMatrix;
	//};


	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		~OrthographicCamera() = default;

		void SetProjectionMatrix(float left, float right, float bottom, float top);
		
		void SetPosition(const glm::vec3& position);
		void SetRotation(float rotation);

		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		inline const float& GetRotation() const { return m_Rotation; }

		void UpdateViewProjectionMatrix();
	private:
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewProjectionMatrix;
		glm::vec3 m_Position;
		float m_Rotation;
	};

	//class Camera3DEuler : public Camera
	//{
	//public:
	//	Camera3DEuler(glm::vec3 position);
	//	~Camera3DEuler();

	//	inline virtual void SetRotation(float rotation) override;

	//	virtual const glm::mat4& GetProjectionMatrix() const override { return m_ProjectionMatrix; }
	//	virtual const glm::mat4& GetViewMatrix() const override { return m_ViewMatrix; }
	//	
	//	virtual void UpdateViewProjectionMatrix() override;
	//private:
	//	float m_Yaw;		// Rotation around Y-axis
	//	float m_Pitch;		// Rotation around X-axis
	//	//float m_Roll;		// Rotation around Z-axis	Not needed in First Person Camera

	//	glm::vec3 m_Position;
	//	glm::vec3 m_Front;
	//	glm::vec3 m_Up;
	//	glm::vec3 m_Right;
	//	glm::vec3 m_WorldUp;

	//	glm::mat4 m_ViewMatrix;
	//	glm::mat4 m_ProjectionMatrix;
	//	glm::mat4 m_ViewProjectionMatrix;
	//};

}